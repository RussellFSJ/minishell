*This project has been created as part of the 42 curriculum by ebin-ahm, rfoo.*

# Minishell

A small Bash-inspired command interpreter written in C for the **42 curriculum**.

> **Status: IN PROGRESS | Last source review: 17 October 2026**  
> The shell's input-to-execution flow and all seven required builtins now have implementations **in source**. Successful compilation, end-to-end behavior, Norminette, memory safety, and file-descriptor handling **have not been verified by this documentation review**. Do not treat the checklist below as passing test results.

## Completion snapshot

| Area | Current state | Next action |
| --- | --- | --- |
| Input → lexer → parser → expander → executor | Connected in `src/minishell.c` | Build and run end-to-end tests |
| `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` | **7/7 implemented in source** | Validate behavior, errors, and persistent state |
| Pipes, PATH, redirections, heredocs | Implementation present | Verify execution, exit statuses, and FD cleanup |
| Signals and `$?` | Implementation present | Verify Ctrl-C/Ctrl-\\ and status propagation |
| Norminette, Valgrind, regression tests | **Not verified** | Run tests and record results for a commit |

### What to do next

1. **P0: Build and smoke-test** the current integrated shell. Fix compilation, crashes, or commands that do not execute.
2. **P0: Test all seven builtins**, especially `cd`, `export`, and `exit` (recently implemented).
3. **P1: Test the executor**: PATH lookup, pipes, `<`, `>`, `>>`, `<<`, and exit statuses.
4. **P1: Test expansion, syntax errors, and signals**, comparing mandatory behavior with Bash.
5. **P1: Finish quality checks**: Norminette, memory leaks, file descriptors, and clean rebuild.

**[Open the step-by-step test checklist →](test.md)**

## Description

Minishell recreates the main mechanics of a Unix shell: reading commands, interpreting quotes and operators, expanding environment variables, and executing builtins or external programs.

```text
readline → lex → parse → expand → execute → cleanup → next prompt
                                  ├── builtin in parent when appropriate
                                  └── fork / pipes / redirects / execve
```

The objective is to satisfy the **mandatory 42 Minishell subject**, not to implement every Bash feature.

## Instructions

### Requirements

- A C compiler and `make`
- GNU Readline headers and library
- Linux / the 42 evaluation environment recommended for final testing

On Debian/Ubuntu, if dependencies are missing:

```bash
sudo apt-get install build-essential libreadline-dev
```

### Build and run

```bash
git clone https://github.com/RussellFSJ/minishell.git
cd minishell
make
./minishell
```

| Command | Action |
| --- | --- |
| `make` | Compile `minishell` |
| `make re` | Rebuild from scratch |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and executable |

**Note:** The current root Makefile has no `make test` target. Follow [`test.md`](test.md) instead.

### Quick smoke test

Type these commands **one at a time inside `./minishell`**:

```bash
echo hello
pwd
/bin/echo external
export TEST42=works
echo "$TEST42"
unset TEST42
echo "<$TEST42>"
echo hello | wc -c
```

**Expected:** `hello`; working directory; `external`; `works`; `<>`; and `6` from `wc -c`. If any step fails, record the failure; these are expected results, **not claims that tests have passed**.

Use [`test.md`](test.md) for complete tests of builtins, pipes, heredocs, errors, signals, and memory.

## Project layout

```text
minishell/
├── Makefile
├── README.md           # Setup, status, overview
├── test.md             # Test instructions and checklist
├── include/            # Shared data types and function declarations
├── libft/              # Supporting C library
└── src/
    ├── main.c         # Entry point
    ├── minishell.c    # Interactive loop and command lifecycle
    ├── lexer/         # Tokenization
    ├── parser/        # Command structure and syntax
    ├── expander/      # Variables, quotes, and $?
    ├── builtins/      # Seven mandatory builtins
    ├── executor/      # Execution, PATH, pipelines, waits
    ├── redirect/      # File redirects and heredocs
    ├── signals/       # Interactive and child signals
    ├── env/           # Persistent environment
    └── cleanup/       # Memory cleanup
```

The directory map is a guide to the main modules; confirm exact paths in the checkout with `find src -type f | sort`.

## 42 development rules

- Maximum **5 functions per `.c` file**, **25 lines per function**, **4 parameters per function**.
- **No ternary operators**; comply with Norminette and the actual subject's allowed functions.
- Compile with `-Wall -Wextra -Werror` (set in the Makefile).
- Prefer `#include "minishell.h"` in project `.c` files.
- Keep shared structure/API changes coordinated: changes to `include/` can affect multiple modules.

## Resources

- [42 Minishell subject](https://projects.intra.42.fr/) (access through 42 Intra) — mandatory requirements and allowed functions.
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/) — syntax, quoting, expansion, and builtin behavior.
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) — interactive input and history.
- [Linux man-pages](https://man7.org/linux/man-pages/) — `fork`, `execve`, `pipe`, `dup2`, `waitpid`, signals, and file APIs.
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html) — reference background; the 42 subject takes precedence.

**AI usage:** ChatGPT was used to explain C and Unix concepts; assist design discussions and static code reviews involving lexer, parser, expander, builtins, execution, cleanup, and error handling; propose edge-case tests; and draft/revise documentation. AI feedback is **not** a substitute for independent implementation, understanding, compilation, and testing by the project authors.

## Authors

`ebin-ahm` · `rfoo`  
[GitHub repository](https://github.com/RussellFSJ/minishell)