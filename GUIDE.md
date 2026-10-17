# 42 Minishell — Handover & Completion Guide

> **Review date:** 17 October 2026  
> **Repository:** https://github.com/RussellFSJ/minishell (`master`)  
> **Audience:** Russell and anyone finishing, debugging, or evaluating this project  
> **Status:** **SOURCE INTEGRATED; PASS NOT YET VERIFIED**

This document is the **handover and completion manual**. The root [`README.md`](README.md) should remain the short project introduction. This guide records **what exists, what still needs work, where to work, how to verify fixes, and what counts as finished**.

> **Evidence warning:** The 17 October audit inspected GitHub source files, but **did not compile or execute the checked-out repository**. An attempted clone was blocked by network/DNS access. Therefore, no new runtime, Norminette, Valgrind, or evaluation pass is claimed here. The original ~67% figure is historical, not a measured score. Record a commit hash and actual test output before checking off any result.

---

## 0. START HERE — current position and next actions

**Current verdict: NOT CLEARED FOR SUBMISSION.** The architecture and all seven mandatory builtins now have implementation code. The main loop is connected to lexer → parser → expander → executor. The remaining work is **correctness, error behavior, interactive signals, regression tests, and cleanup**, not writing the whole shell again.

### Completion dashboard

| Area | Source status (17 Oct) | Runtime evidence | Next action |
|---|---|---|---|
| Startup and readline | Present | **Not run** | Clean build; EOF, Ctrl-C, history |
| Lexer | Present | Historical standalone testing; **not rerun** | Quotes, operators, malformed input |
| Parser | Present | **Not run** | **Fix syntax error classification** |
| Expansion | Present | **Not run** | **Audit word splitting and empty arguments** |
| Environment | Present | **Not run** | Export/unset state and ownership |
| 7 mandatory builtins | **7/7 implemented in source** | **Not run** | Edge-case verification; parent vs child |
| Executor/PATH/pipelines | Present and called | **Not run** | Status, errors, wait/reap, FD handling |
| Redirections/heredocs | Present | **Not run** | Ordering, cleanup, signals, EOF |
| Interactive signals | Present | **Not run** | SIGINT/SIGQUIT behavior and safety |
| Norminette | Unknown | **Not run** | Run on all submitted `.c`/`.h` files |
| Valgrind / FD audit | Unknown | **Not run** | No definitely lost leaks; no FD growth |
| 42 evaluation readiness | **Unverified** | **No current pass report** | Finish acceptance gate in §10 |

**Do not confuse 7/7 builtins written with 7/7 builtins passing.** No credible grade or overall completion percentage can be assigned from static code alone.

### The next five tasks, in order

| Priority | File(s) | Task | Done when |
|---|---|---|---|
| **P0-1** | `src/parser/parser.c`, `src/minishell.c` | Distinguish syntax failures from allocation failures | `echo a \|` prints syntax error and `$?` is `2` |
| **P0-2** | `src/expander/expander.c`, `src/expander/expander_utils.c` | Verify quote removal, unquoted `$VAR` splitting, empty variable behavior | Bash comparison cases in §8 pass |
| **P0-3** | Whole repository | Clean `make re`, then core regression suite | Required commands run without crashes, hangs or wrong status |
| **P1-4** | `src/signals/`, `src/redirect/`, `src/executor/` | Interactive Ctrl-C/Ctrl-\\, heredoc and pipeline/FD audit | PTY/manual signal checks + repeat tests pass |
| **P1-5** | Whole repository, Makefile | Norminette, Valgrind, open FD checks, allowed functions, docs | Evidence recorded for same clean commit |

**Suggested ownership:** Russell drives remaining execution, builtin edge cases, and integration. The original frontend contributor can review lexer/parser/expander contracts and expansion fixes. Shared headers/interfaces require both reviewers.

---

## 1. How to take over the project

```bash
git clone https://github.com/RussellFSJ/minishell.git
cd minishell
git switch master
git pull --ff-only
git status --short
git rev-parse HEAD
make fclean && make
./minishell
```

The root Makefile currently compiles `src/**/*.c` (discovered with `find`), builds `libft/libft.a`, links with `-lreadline`, and provides `all`, `clean`, `fclean`, `re`. **It does not currently expose a `make test` target.** The machine must have readline headers/library available. On a Mac, check Homebrew readline paths; validate the finished project on the 42 evaluation platform.

**Fast smoke test inside `./minishell`:**

```bash
echo hello
/bin/echo external
pwd
export HANDOVER=ready
echo "$HANDOVER"
false
echo $?
echo hello | wc -c
exit 0
```

Do not mark these as passing until someone observes the output and exit codes. The optional [`minishell_check.py`](minishell_check.py) automates non-interactive Bash comparisons, but cannot replace terminal signal tests.

### Ground rules (42 + team conventions)

- **Maximum 5 functions per C file; 25 lines per function; no ternary operations.** Respect the subject's actual Norminette and allowed-function requirements; the additional team convention is **at most 4 parameters per function**.
- Compile with `-Wall -Wextra -Werror`. Prefer only `#include "minishell.h"` in project `.c` files where practical.
- Never rename shared structs, fields, exported functions, return contracts or headers without checking all callers.
- Avoid unintended globals; the existing `g_signal` is for signal state only.
- **Every allocation, fork, pipe, open, dup/dup2, exec and wait path must have failure handling and ownership rules.**
- Keep tests and test outputs separate from submitted C code; never leave debug `printf`s in normal execution paths.

---

## 2. Execution lifecycle — how the pieces fit

```text
main.c: main() -> minishell(envp)
                  |
                  v
minishell.c: shell_init() -> readline() + history
                  |
                  v
lexer/:       lex(line, &error)              -> t_token list
                  |
                  v
parser/:      parse(tokens, &error)          -> t_command list
                  |   free_tokens(tokens)
                  v
expander/:    expand(shell, commands)        -> mutate argv/redir targets
                  |
                  v
executor/:    execute(shell, commands)
                  |  collect heredocs
                  |  one standalone builtin -> parent + stdio restore
                  |  otherwise -> pipeline_run() -> fork/pipe
                  |                         -> redirect_apply()
                  |                         -> builtin_dispatch() or execve()
                  |  wait/reap; close heredoc FDs
                  v
shell.last_status updated; free_commands(commands)
                  |
                  v
next prompt (until shell.should_exit or EOF)
```

### Why these boundaries exist

| Decision | Reason | What can break if changed |
|---|---|---|
| Parser creates its own command strings | Tokens can be freed before execution | Use-after-free if `argv` borrows token memory |
| Expansion mutates command-owned data | Later executor sees final strings | Leaks/double-free if old strings not replaced safely |
| Standalone builtin runs in parent | `cd`, `export`, `unset`, `exit` must change persistent shell state | Running always in a child loses updates |
| Builtin in pipeline runs in child | Pipeline commands should not mutate parent state | `export A=1 \| cat` must not persist `A` |
| Heredocs collected before execution | Prompts must be resolved before running pipeline | Deadlocks, incorrect signal handling |
| Parent closes pipe ends and reaps every child | Prevent deadlocks and zombies | `yes \| head -n 1` may hang |
| `last_status` lives in `t_shell` | `$?` uses most recent command's result | Wrong status after errors/signals |

**Ownership rule:** `t_shell.env` survives across prompts. Each command line owns its input buffer, tokens, command list, expanded values and temporary FDs. Tokens must be freed after parsing; commands must be freed after execute or expansion error; environment is freed when Minishell exits. Do not `free()` a pointer returned by `env_get()` because it is borrowed from the environment list.

---

## 3. Source map and function contracts

### Where to work

| Module | Key files | Responsibility |
|---|---|---|
| Entry / loop | `src/main.c`, `src/minishell.c` | Shell lifetime, signal mode, prompt, frontend errors |
| Environment | `src/env/`, `include/env.h` | Init, get/find, set/unset, export flags, envp conversion |
| Lexer | `src/lexer/`, `include/lexer.h` | Word/quote/operator tokenization |
| Parser | `src/parser/`, `include/parser.h` | Syntax checks, command/argv/redirection structure |
| Expander | `src/expander/`, `include/expander.h` | `$VAR`, `$?`, quote removal, redirect targets |
| Builtins | `src/builtins/`, `include/builtins.h` | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| Executor | `src/executor/`, `include/executor.h` | Parent builtins, PATH, child run, pipe/fork/wait, statuses |
| Redirections | `src/redirect/` | `<`, `>`, `>>`, `<<`, temp FD cleanup |
| Signals | `src/signals/` | Prompt, wait, child, heredoc signal contexts |
| Cleanup | `src/cleanup/`, `include/cleanup.h` | Free tokens, commands, redirections and env |
| Shared data | `include/types.h`, `include/minishell.h` | Structures and common declarations |

Run `find src -type f -name '*.c' | sort` on the checkout for the **complete current** source map. Do not assume the selected files above are the full list.

### High-impact APIs and their relationships

| API | Called from / consumed by | Return and safety contract |
|---|---|---|
| `env_init(envp)` | `shell_init` | Returns owned environment list or NULL. NULL may also represent a genuinely empty env; startup should distinguish this from OOM if relevant. |
| `env_get(env, key)` | expander, `cd`, PATH | Borrowed value or NULL; never free returned pointer. |
| `env_set(&env, key, value)` | `cd`, `export` | 0 on success, nonzero on error; may replace old value and mark exported. |
| `env_unset(&env, key)` | `unset` | Removes/updates owned nodes without invalidating traversal. |
| `env_to_array(env)` | `child_exec` | Freshly allocated `char **`; free on failed exec. |
| `lex(line, &error)` | `process_line` | Token list or NULL; lexer frees partially built tokens on reported failure. |
| `parse(tokens, &error)` | `process_line` | Owned commands or NULL; parser frees partial commands on error. |
| `expand(shell, cmds)` | `process_line` | 0 success, -1 allocation/expansion error; argument/redir ownership remains with command. |
| `is_builtin(name)` | `execute`, `child_run` | Recognizes seven builtin names, null-safe. |
| `builtin_dispatch(shell, cmd)` | parent or child | Returns builtin command status; persistent changes matter only in parent. |
| `execute(shell, cmds)` | `process_line` | Collect heredocs, choose parent/pipeline, update `last_status`, close heredoc FDs. |
| `path_resolve(env, name)` | `child_exec` | Newly allocated path or NULL, caller owns it; distinguish ENOENT/EACCES/ENOMEM. |
| `pipeline_run(shell, cmds)` | `execute` | Manage pipe/fork/wait; status from **last** pipeline command. |
| `redirect_apply(cmd)` | parent builtin, child | Apply redirections left-to-right, fail cleanly on open/dup2. |
| `heredoc_collect(shell, cmds)` | `execute` | Prepare all heredocs; preserve interrupt/error status and close on failure. |
| `free_tokens`, `free_commands`, `free_env` | frontend / shutdown | Free exactly once at the appropriate lifetime boundary. |

### Shared data invariants

- `t_token`: `type`, `value`, `next`; token text belongs to lexer token until freed.
- `t_redir`: `type`, `target`, `expand_body`, `fd`, `next`. `fd == -1` means no active heredoc FD; close valid descriptors before freeing nodes.
- `t_command`: `argv`, `redirs`, `next`; argv is NULL-terminated where present; a redirection-only command can have no argv.
- `t_env`: `key`, `value`, `exported`, `next`; distinguish absent value (`NULL`) from `""`, and exported from non-exported.
- `t_shell`: `env`, `last_status`, `should_exit`; shell state must survive prompt-to-prompt.

---

## 4. Confirmed findings and investigation queue

These are **static inspection findings**, not claims of observed test failures. Fix **confirmed logic mismatches** first, then test the suspected edge cases.

| ID | Priority | Location | Finding | Fix / evidence needed |
|---|---|---|---|---|
| **G-01** | **P0** | `src/parser/parser.c` → `src/minishell.c` | Parser sets `error = -1` when `parser_check_syntax()` rejects an operator; `frontend_error()` interprets `-1` as **memory allocation failed**, reserving `-2` for syntax. | Use a consistent error enum/contract. Test `\| cat`, `echo hi \|`, `cat >` and `$? == 2`. |
| **G-02** | **P0** | `src/expander/expander_utils.c` | `expand_argv()` substitutes **one string per original argument**, without producing multiple fields or removing an unquoted empty field. | Decide behavior according to subject/Bash requirements; implement quote-sensitive field splitting if required, then test quoted/unquoted variables. |
| **G-03** | **P1** | `src/signals/signals.c` | SIGINT handler directly calls `rl_on_new_line()`, `rl_replace_line()` and `rl_redisplay()`, which are not async-signal-safe. | Prefer signal flag + safe deferred Readline refresh where practical. Verify Ctrl-C at prompt, during command and heredoc. |
| **G-04** | **P1** | `src/builtins/builtin_cd.c` | An `env_set()` failure ends in `perror("minishell: cd")` even though it may not set `errno`; can print an unrelated OS error. `getcwd()` failure before `chdir()` also aborts immediately. | Print accurate allocation/update error; test unset HOME, invalid path, deleted cwd, PWD/OLDPWD. |
| **G-05** | **P1** | `src/builtins/builtin_export_print.c` | Exported string values are wrapped in `"` but embedded quotes/backslashes are not escaped. | Compare `export` output after values containing quotes, backslashes, newlines. |
| **G-06** | **P1** | `src/executor/executor_path.c` | When PATH is unset, resolver duplicates bare command name rather than providing an explicit search policy; relative bare-name execution may differ from Bash. | Test `unset PATH`, empty PATH, `/bin/ls`, `./tool`, missing command, directory and permission cases. |
| **G-07** | **P1** | `src/redirect/heredoc.c`, `src/executor/` | Heredoc temp FD lifecycle and interrupts need runtime/Valgrind proof; pipeline code has several partial-failure paths. | Loop heredocs, interrupt them, force bad redirects, ensure no leaked descriptors or zombies. |
| **G-08** | **P1** | `src/minishell.c` | `env_init()` can return NULL for an empty environment **or** an allocation failure; `shell_init()` does not distinguish the two. | Test `env -i ./minishell`; plan explicit OOM/initialization error handling. |
| **G-09** | **P1** | Makefile / repository | No current automated test target and no current reproducible result log. | Run `minishell_check.py`, manual PTY tests, Norminette and memory checks; record commit and results. |

**Important corrections to older handover notes:** `src/minishell.c` **does** connect the frontend to `execute()`. `cd`, `export`, and `exit` **are no longer stubs**. The old tasks “implement the three missing builtins” and “wire main loop” should be removed. Also, `builtin_export()` already preserves the value of an existing key for `export KEY` without an assignment; do not report that exact case as a confirmed bug.

---

## 5. Ordered completion work for Russell

### Phase A — establish a reliable baseline (P0)

1. `make fclean && make`, then run `norminette src include` (and `libft` if in evaluation scope).
2. Record `git rev-parse HEAD`, OS, compiler and readline version, and the full build result.
3. Run the quick smoke test in §1 and `python3 minishell_check.py` if copied into the repository root.
4. Create a simple gap log: **test ID, observed output, expected output, reproducible command, file owner, fix commit**.
5. Fix `G-01` first; otherwise diagnostics and `$?` contaminate all following error tests.

### Phase B — finish shell behavior (P0)

1. **Expansion**: single quotes literal; double quotes expand; `$?`; empty strings; unquoted splitting; space-containing values; quote removal; ambiguous redirect names.
2. **Builtins in parent**: `cd`, `export`, `unset`, `exit` must persist state. Check `exit 42`, invalid numeric, too many args, negative/overflow.
3. **Commands/PATH**: absolute paths, relative paths, empty/unset PATH, permission denied (126), missing command (127), directories.
4. **Pipelines**: 2+ commands, builtin in pipeline, SIGPIPE producer, all children reaped, status of last command.
5. **Redirections**: apply order; restore stdin/stdout after builtin; redirect-only commands; multiple redirects; read/write failures.

### Phase C — harden the runtime (P1)

1. **Signals**: Ctrl-C clears current prompt (`$? = 130`), child interrupted status 130, Ctrl-\\ during foreground command typically 131, heredoc interrupt returns to prompt.
2. **Heredocs**: quoted vs unquoted delimiters, body variable expansion, EOF warning, multiple heredocs, no leaked temporary files/descriptors.
3. **Memory and FDs**: 100+ commands in one shell, long pipelines, repeated redirections, failed allocation/error paths where feasible.
4. **Norminette**: 5 functions/file, 25 lines/function, 4 parameter team limit, no ternaries, permitted functions and headers.
5. **Final review**: both partners perform independent clean clone / launch / evaluation rehearsal.

**Fix policy:** change the smallest module necessary; don't rewrite passing lexer/parser/environment code to solve an executor test. Update this guide's gap table on every merged fix.

---

## 6. Builtin completion contract

| Builtin | Must handle | Suggested commands |
|---|---|---|
| `echo` | `-n` combinations, empty args, quoted text | `echo hello`; `echo -nnn hello`; `echo "a b"` |
| `cd` | HOME, `..`, invalid path, PWD/OLDPWD, too many args | `pwd`; `cd ..`; `pwd`; `cd /does/not/exist`; `cd a b` |
| `pwd` | Current location, after `cd`, broken cwd | `pwd`; `cd /tmp`; `pwd` |
| `export` | Valid/invalid identifiers, `KEY`, `KEY=`, `KEY=VALUE`, sorted listing, persistence | `export A=42`; `echo "$A"`; `export A`; `export 1BAD=x`; `export` |
| `unset` | Existing, absent, multiple keys, not affecting unrelated keys | `unset A`; `echo "$A"`; `unset DOES_NOT_EXIST` |
| `env` | Only exported values with values, updated values | `export A=42`; `env` |
| `exit` | No args, signed/unsigned numeric, modulo 256, invalid, overflow, too many args | `exit 42`; `exit -1`; `exit abc`; `exit 1 2` |

All seven have source implementations; **none has a fresh end-to-end pass recorded in this guide**.

---

## 7. Return codes, errors, and cleanup

| Situation | Expected status | Expected message / effect |
|---|---|---|
| Command succeeds | `0` | Normal output |
| Builtin / redirect failure | usually `1` | Useful stderr with context |
| Shell syntax error | `2` | Syntax error, **not** allocation failure |
| Command name not found | `127` | `command not found` |
| Found but cannot execute | `126` | Permission denied / directory / exec error |
| Child killed by SIGINT | `130` | Return to interactive prompt |
| Child killed by SIGQUIT | `131` | Return to prompt; terminal-specific display |
| `exit -1` | `255` | Shell exits |
| `exit 256` | `0` | Shell exits |

Diagnostics policy: use `perror()` **for failed syscalls with meaningful `errno`** (`open`, `chdir`, `fork`, `pipe`, `dup2`, `execve`). Use `ft_putstr_fd`/`ft_putendl_fd` for shell-defined messages (invalid identifier, too many arguments, syntax error, allocation failure), rather than stale `errno`. Never print debug logs to stdout.

Cleanup checkpoints: on tokenization failure lexer frees its partial list; parser frees its partially built commands; frontend frees tokens after parse; after expansion/execute frontend frees commands; executor closes heredoc FDs; parent restores stdio after builtin redirects; at shutdown shell frees environment and Readline history.

---

## 8. Targeted tests: run these before submission

### Core and expansion

```bash
echo hello
/bin/echo external
echo 'literal $HOME'
echo "expanded $HOME"
false
echo $?
export CHECK='one two'
printf '<%s>\n' $CHECK      # Bash: two fields
printf '<%s>\n' "$CHECK"    # Bash: one field
unset CHECK
echo "$CHECK"              # Empty line
```

### Parsing and syntax

```bash
| cat                       # Syntax error, status 2
echo hi |                   # Syntax error, status 2
cat >                       # Syntax error, status 2
echo "unfinished             # Quote error; no crash
```

### Execution and FDs

```bash
echo abc | wc -c
printf 'a\nb\n' | grep b | wc -l
cat < /etc/hostname
/bin/echo first > /tmp/minishell-test-output
/bin/echo second >> /tmp/minishell-test-output
cat /tmp/minishell-test-output
cat <<EOF
hello
EOF
```

**Important:** Run status-sensitive cases individually or check `$?` **immediately** after them; the next command overwrites status. Avoid tests that depend on OS-specific paths such as `/etc/hostname` when running on platforms that lack them. The scripted harness uses a private temporary directory.

### Interactive terminal tests (manual PTY/terminal, not stdin pipe)

| Test | Expected |
|---|---|
| Ctrl-D at empty prompt | Exit normally without crash |
| Ctrl-C at empty prompt | New clean prompt; `$?` should be 130 |
| `sleep 10` then Ctrl-C | Child stops, shell survives, `$?` is 130 |
| `sleep 10` then Ctrl-\\ | Child stops, shell survives, `$?` typically 131 |
| `cat <<EOF` then Ctrl-C | Heredoc interrupted, prompt resumes, no FD growth |
| `yes \| head -n 1` | No indefinite hang; pipe ends closed |
| `export KEEP=42` then `echo "$KEEP"` | Parent retains environment change |
| `cd /tmp` then `pwd` | Parent working directory changes |

---

## 9. Tests, Norminette, Valgrind, and evidence

### Quick automatic regression

Copy [`minishell_check.py`](minishell_check.py) into the repository root and run:

```bash
make re
python3 minishell_check.py
```

The script compares **stdout and return status** with Bash across core commands, quoting, expansion, builtins, pipes, redirections and syntax. It removes the known `minishell$ ` prompt from captured stdout. It does **not** compare stderr strings (which can legitimately differ in wording); it prints stderr when a test fails. The shell's EOF `exit` diagnostic is therefore ignored for comparison. **It is an aid, not the official 42 evaluator**. Any FAIL needs a reproducer confirmed in an interactive shell.

### Norminette

```bash
norminette src include
# Also check other required code/libft as dictated by the school rules.
```

### Memory / descriptor checks (Linux / Valgrind available)

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --track-origins=yes --errors-for-leak-kinds=all \
  --error-exitcode=42 ./minishell
```

Run a sequence of commands, redirects, failed paths, and heredocs before exiting. Readline may keep **reachable** allocations that need separate interpretation; **definitely lost, invalid reads/writes, double frees and unclosed FDs are not acceptable**. Check whether your Valgrind build supports `--track-fds=yes`. On macOS, use the available sanitizer/leaks tools but perform final Valgrind testing on a supported Linux machine.

### Record results for every handover commit

```text
Commit SHA:   ______________________________
Date / OS:    ______________________________
make re:      PASS / FAIL / NOT RUN
Norminette:   PASS / FAIL / NOT RUN
Regression:   __ passed / __ failed / __ skipped
Interactive:  PASS / FAIL / NOT RUN
Valgrind:     PASS / FAIL / NOT RUN
FD audit:     PASS / FAIL / NOT RUN
Open defects: ______________________________
Reviewer(s):  ______________________________
```

Do not reuse results from a different commit as evidence of current readiness.

---

## 10. Definition of done — the final gate

**Mark project PASS-READY only if all of these are proven on the same clean revision:**

- [ ] `make fclean && make` exits 0 with `-Wall -Wextra -Werror`, without relying on stale object files.
- [ ] Norminette and project-specific rules pass.
- [ ] The seven builtins pass their mandatory cases and preserve parent shell state where required.
- [ ] Lexer, parser, quote handling, variable expansion and error status tests pass.
- [ ] Absolute/relative/PATH commands, 2+ pipelines and last-command statuses pass.
- [ ] `<`, `>`, `>>`, `<<`, redirection-only commands and error cases pass.
- [ ] Ctrl-C, Ctrl-\\, Ctrl-D and heredoc interruption behave correctly in an interactive terminal.
- [ ] No invalid memory access, lost allocations, FD leaks, zombies or hangs in required test paths.
- [ ] The Makefile has working `all`, `clean`, `fclean`, `re` and required rebuild behavior.
- [ ] README meets current 42 subject requirements and points to this `guide.md`.
- [ ] Both partners can explain their own modules, shared data, fork/pipe/FD ownership and failure cleanup.
- [ ] Test evidence and the exact Git commit hash are recorded above.

**Current gate result (17 Oct source audit): NOT YET PASSED.** This is a lack of current execution evidence plus known source issues, **not** a claim that all tests fail.

### Questions each partner should be able to answer at evaluation

1. Why must `cd`, `export`, `unset` and standalone `exit` run in the parent?
2. Who owns token strings versus parsed `argv` strings? When are they freed?
3. Which descriptor ends must parent and children close for `a | b | c`?
4. Why is the last pipeline command responsible for `$?`?
5. Why must heredocs be prepared and their FDs cleaned up?
6. How do quoted and unquoted `$VAR` differ? What about `$?` and empty expansion?
7. What is the difference between shell diagnostics and `perror()`?
8. How are command-not-found (127), not-executable (126), and syntax (2) differentiated?
9. What happens when `fork`, `pipe`, `dup2`, `malloc`, `open` or `execve` fails?
10. How does the shell avoid losing parent state when executing a builtin?

---

## 11. Collaboration and handover protocol

1. Pull `master`, record SHA, and reproduce the issue before editing.
2. Assign one file/module owner per concurrent fix; agree before changing `include/types.h`, public headers or status conventions.
3. Keep changes scoped and Norminette-safe: **5 functions/file, 25 lines/function, no ternaries**.
4. Run strict build + targeted case(s) + related regressions. Use Valgrind for ownership/FD-related fixes.
5. Commit with intent, e.g. `fix(parser): report syntax errors with status 2`.
6. Update the **G-XX** finding with reproduction, fix SHA and actual result; remove resolved gaps from the top dashboard.
7. Re-run the complete final gate after merging. A previous clean result becomes historical whenever relevant source changes.

### References

- [42 Minishell repository](https://github.com/RussellFSJ/minishell)
- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [GNU Readline documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)
- [Linux `fork(2)`](https://man7.org/linux/man-pages/man2/fork.2.html), [`pipe(2)`](https://man7.org/linux/man-pages/man2/pipe.2.html), [`dup2(2)`](https://man7.org/linux/man-pages/man2/dup.2.html), [`execve(2)`](https://man7.org/linux/man-pages/man2/execve.2.html)
- [Valgrind Memcheck Manual](https://valgrind.org/docs/manual/mc-manual.html)

**Document maintenance:** This is a source-audit handover document. Update its findings and results whenever a new tested revision is merged. Do not silently turn "not run" into "pass".