*This project has been created as part of the 42 curriculum by ebin-ahm, rfoo.*

# Minishell

A Bash-inspired command interpreter written in C for the 42 curriculum.

> **Status (17 October 2026): IN PROGRESS — not submission-ready.**
> `src/main.c` currently reads and discards commands; `cd`, `export`, and `exit`
> remain stubs. The tests below are **instructions**, not verified passing results.

## What to do next

| Priority | Task | Done when |
| --- | --- | --- |
| **P0** | Connect lexer → parser → expander → executor in `src/main.c` | `echo hello` runs inside `./minishell` |
| **P0** | Implement `cd`, `export`, `exit` in `src/builtins/` | Tests B and F pass |
| **P1** | Validate pipes, redirections, heredocs, PATH, signals | Tests A–F pass |
| **P1** | Fix errors, leaks, FDs, Norminette | Final checks pass |

## Description

Minishell reads a command line, splits it into tokens, parses commands,
expands variables, and runs builtins or executables using Unix processes.
It aims to implement the **mandatory 42 Minishell subject**, not all of Bash.

```text
readline → lex → parse → expand → execute → cleanup → next prompt
                                    ↳ builtin / fork + execve
```

## Instructions

### Build and run

Requires `make`, a C compiler and GNU Readline. On Debian/Ubuntu, install
`build-essential` and `libreadline-dev` if needed.

```bash
git clone https://github.com/RussellFSJ/minishell.git
cd minishell
make re
./minishell
```

| Command | Purpose |
| --- | --- |
| `make` | Build `minishell` |
| `make re` | Clean rebuild |
| `make clean` / `make fclean` | Remove build files / executable |

**Do not use `make test`:** the current root Makefile has no test target.

### Make commands execute (first milestone)

In `src/main.c`, implement the following flow for each non-empty line:

1. `lex(line, &error)` → check errors.
2. `parse(tokens, &error)` → free tokens when finished.
3. `expand(shell, commands)` → check errors.
4. `execute(shell, commands)` → update last exit status.
5. Free commands and input on **every** success/error path.

Keep `shell.env` alive between prompts. Preserve the existing function
interfaces; do not accidentally free the shell's persistent environment.

## Test checklist: commands and expected results

**Run each block inside `./minishell`, after main-loop integration.**
Do not paste shell comments or the Markdown fences into Minishell.

### A. External commands and exit status

```bash
echo hello
pwd
/bin/echo absolute
/bin/false
echo $?
missing_command_42
echo $?
```

**Expect:** `hello`, the current path, `absolute`, status `1`, then a
command-not-found error and status `127`.

### B. Builtins and persistent state

```bash
export TEST42=hello
echo "$TEST42"
env | grep '^TEST42=hello$'
unset TEST42
echo "<$TEST42>"
cd /tmp
pwd
cd /does/not/exist
echo $?
```

**Expect:** Variable survives between prompts; `unset` removes it (`<>`);
`pwd` becomes `/tmp`; invalid `cd` reports an error and returns nonzero.
Separately test `echo -n`, `export` (no arguments), `cd` (no arguments),
`exit`, `exit 42`, `exit abc`, and `exit 1 2`.

### C. Quotes and expansion

```bash
echo '$HOME'
echo "$HOME"
echo "a    b"
echo ""
echo '$?'
```

**Expect:** Single quotes prevent expansion; double quotes allow it;
quoted spaces and empty arguments are preserved.

### D. Pipes and file redirection

```bash
echo hello | wc -c
echo first > /tmp/minishell_test.txt
echo second >> /tmp/minishell_test.txt
cat < /tmp/minishell_test.txt
cat /tmp/minishell_test.txt | grep second
```

**Expect:** `6`; file contains two lines (`first`, `second`);
`grep` prints `second`; no hanging pipe.

### E. Heredoc and errors

Enter this as a multiline heredoc:

```bash
cat << STOP
hello
STOP
```

**Expect:** `hello`. Repeat with `cat << 'STOP'` and `$HOME` in its body:
quoted delimiter = no expansion; unquoted delimiter = expansion.

Test each error as its own command:

```bash
| cat
echo hello >
cat < /does/not/exist
```

**Expect:** Error message, nonzero `$?`, and a usable next prompt.

### F. Signals and exit

| Test | Expected |
| --- | --- |
| Ctrl-C at empty prompt | New prompt; shell remains open |
| Ctrl-\ at empty prompt | Ignored |
| Ctrl-D at empty prompt | Exit shell |
| `sleep 10` then Ctrl-C | Interrupted; `$?` = `130` |
| `sleep 10` then Ctrl-\ | Interrupted; `$?` = `131` |
| Ctrl-C inside heredoc | Cancel heredoc and return to prompt |
| `exit 42` (new run) | Shell exits with code `42` |

For the last row, type `echo $?` in the **parent terminal** after Minishell exits.

### Final checks (run from the parent terminal)

```bash
make re
norminette src include libft
valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell
```

Under Valgrind, rerun A–E and exit. Look for **invalid memory access,
project-owned leaks, and open file descriptors**. Valgrind is for supported
Linux environments; inspect Readline/library allocations separately.

Compare required behavior against Bash, for example:

```bash
printf '%s\n' 'echo "a b"' 'echo "$?"' | bash --noprofile --norc
printf '%s\n' 'echo "a b"' 'echo "$?"' | ./minishell
```

Ignore Minishell's prompt text when comparing output. Record the commit under
test with `git rev-parse --short HEAD`.

## Fix failures here

| Failure | Check |
| --- | --- |
| Typed commands do nothing | `src/main.c` |
| Quotes / syntax wrong | `src/lexer/`, `src/parser/` |
| `$VAR`, `$?` wrong | `src/expander/` |
| `cd`, `export`, `exit` wrong | `src/builtins/` |
| PATH / exit codes / pipes wrong | `src/executor/` |
| `<`, `>`, `>>`, `<<` wrong | `src/redirect/` |
| Ctrl-C / Ctrl-\ wrong | `src/signals/` |
| Leaks / FDs / crashes | `src/cleanup/`, error paths in all modules |

## 42 rules

Maximum **5 functions per file**, **25 lines per function**, **4 parameters per
function**; **no ternary operators**. Follow Norminette and compile with
`-Wall -Wextra -Werror`. Prefer `#include "minishell.h"` in project C files.

## Resources

- **Official 42 Minishell subject (campus-provided PDF)** — feature and evaluation requirements.
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/) — quotes, builtins, expansion, exit status.
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) — prompt and history.
- [Linux man-pages](https://man7.org/linux/man-pages/) — `fork`, `execve`, `pipe`, `dup2`, `waitpid`, `open`, signals.
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html) — background reference; follow the 42 subject's scope.

**AI usage:** ChatGPT assisted with C/system-call explanations, design and code
review for lexer, parser, expander, builtins, executor and cleanup; suggested
edge-case tests, error-handling checks, and documentation. The authors must
understand and independently test all implemented code.

## Authors

`ebin-ahm` · `rfoo`  
https://github.com/RussellFSJ/minishell