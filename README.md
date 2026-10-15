# 42 Minishell | Project Handover and Completion Guide

> **Handover reference | 15 October 2026 | Branch: `master`**  
> Repository: https://github.com/RussellFSJ/minishell  
> **Status: implementation in progress; not submission-ready.**
>
> This guide replaces the previous August handover notes. It distinguishes code that exists from behavior that has been demonstrated in a running, integrated shell. The old **~67%** figure (30 August 2026) was a historical planning estimate, **not a current functional-completion score or a 42 grade**.
>
> **Verification scope:** based on the repository's accessible `master` README, headers, Makefile, and selected source files reviewed on 15 October 2026. This is a **static review**. A local clone, compilation, full tree audit, Norminette run, and current Valgrind run were **not** completed as part of this update. Reconfirm every item against the latest checkout before marking it complete.

---

## Project status | Quick overview

> **OVERALL: NOT SUBMISSION-READY**  
> **Immediate blockers: 2** (main-loop integration; 3 unfinished builtins)  
> **Verified end-to-end shell: NO**  
> **Completion percentage: NOT VERIFIED.** The previous **~67%** was a 30 August 2026 planning estimate, not the present test-backed completion rate.

### At-a-glance gap tracker

| Area | Status | Main gap |
| --- | --- | --- |
| Environment, lexer, parser | **Source implemented** | Run integrated syntax, quoting, ownership and error-path tests |
| Expander | **Source implemented; audit needed** | Verify empty variables, quoted/unquoted expansion and redirect targets |
| Mandatory builtins | **4/7 have core implementations** | Finish `cd`, `export`, `exit` (currently stubs) |
| Executor, pipes, PATH, redirects, heredocs, signals | **Source implemented; integration unverified** | Test behavior, exit codes, FD closure, interrupts and failures |
| Interactive shell (`src/main.c`) | **BLOCKED** | Connect `readline()` to lex → parse → expand → execute |
| Automated tests, Norminette, memory/FD checks | **Not currently verified** | Add runnable regression suite and save evidence by commit |

### Next actions | Do these in order

| Priority | Where | What to do now | Mark done when… |
| --- | --- | --- | --- |
| **P0 · 1** | `src/main.c` + frontend APIs | Add per-line lifecycle: lex, parse, free tokens, expand, execute, free commands; propagate errors/status | `echo hello`, `pwd`, `/bin/echo hi`, `false`, `echo $?` work in `./minishell` |
| **P0 · 2** | `src/builtins/builtin_cd.c` | Implement directory changes, `HOME`, failures, `PWD`/`OLDPWD` | `cd`, `cd ..`, bad paths and subsequent `pwd` work |
| **P0 · 3** | `src/builtins/builtin_export.c` | Implement identifiers, assignments and no-argument listing | Across separate prompts, `export X=42`, `env`, `echo $X`, and `unset X` behave correctly |
| **P0 · 4** | `src/builtins/builtin_exit.c` | Implement numeric arguments, overflow, too many args, correct exit state | `exit`, `exit 42`, invalid numeric and excess arguments work |
| **P1 · 5** | `src/executor/*`, redirects, heredocs, signals | **Test and fix existing code**, rather than rewriting it; audit `pipe`, `fork`, `dup2`, `waitpid`, signal/FD lifecycles | Pipelines, `< > >> <<`, Ctrl-C and last-command statuses match required Bash behavior |
| **P1 · 6** | `src/expander/*`, parser | Verify quoting, `$?`, empty/unquoted expansions and ambiguous redirects | Defined behavior passes regression comparisons without crashes |
| **P1 · 7** | `Makefile`, test setup, all modules | Restore runnable tests; run strict build, Norminette, memory and FD checks | Results recorded against a specific clean commit; all mandatory checks pass |

**First milestone:** make a basic command travel from the prompt through the existing execution path. **Second:** complete the three missing builtins. **Third:** harden the existing runtime and prove it with tests. See [Section 10](#10-ordered-completion-plan) for the step-by-step plan and [Section 16](#16-definition-of-done) for final acceptance criteria.

**Status rule:** “source implemented” is **not** “tested and complete.” Tick off tasks only with reproducible build/run evidence from the current commit.

---

## Contents

[Project status | Quick overview](#project-status--quick-overview)

1. [Handover at a glance](#1-handover-at-a-glance)
2. [42 rules and team conventions](#2-42-rules-and-team-conventions)
3. [How to build and launch](#3-how-to-build-and-launch)
4. [Architecture and execution lifecycle](#4-architecture-and-execution-lifecycle)
5. [Current module status](#5-current-module-status)
6. [Repository map and public function contracts](#6-repository-map-and-public-function-contracts)
7. [Shared structures, ownership, and invariants](#7-shared-structures-ownership-and-invariants)
8. [Design choices that must be preserved](#8-design-choices-that-must-be-preserved)
9. [Known blockers and review risks](#9-known-blockers-and-review-risks)
10. [Ordered completion plan](#10-ordered-completion-plan)
11. [Builtins: requirements and acceptance tests](#11-builtins-requirements-and-acceptance-tests)
12. [Execution, redirection, heredoc, and signals](#12-execution-redirection-heredoc-and-signals)
13. [Testing and memory/FD validation](#13-testing-and-memoryfd-validation)
14. [Review checklist and collaboration protocol](#14-review-checklist-and-collaboration-protocol)
15. [Evaluation questions](#15-evaluation-questions)
16. [Definition of done](#16-definition-of-done)

---

## 1. Handover at a glance

**Primary handover objective:** finish and verify the existing implementation without unnecessarily rewriting the lexer, parser, environment layer, or shared interfaces.

**Current important facts:**

- The `main.c` loop currently initializes the shell, reads input, records history, and frees the input. **It does not yet call `lex()`, `parse()`, `expand()`, or `execute()`.** Therefore the executable is not yet an operational command interpreter.
- The executor is **not merely a stub anymore**: `execute()`, `pipeline_run()`, `child_run()`, `path_resolve()`, `redirect_apply()`, `heredoc_collect()`, and signal setup functions now contain implementation code. They still require integrated execution and hardening.
- `builtin_cd()`, `builtin_export()`, and `builtin_exit()` remain true stubs: they currently return `0` without carrying out the requested operation.
- The environment, lexer, parser, expander, builtin detection/dispatch, and cleanup modules form the existing foundation. Preserve their contracts, but **do not claim final correctness until integration and regression tests pass**.
- The current root `Makefile` builds `minishell`, but **does not define** `make tests`, `make test_parser`, or other `test_*` targets described by the old README. Reintroduce a test harness/targets explicitly or run a separate tester.

### Handover priority

| Priority | Deliverable | Completion evidence |
| --- | --- | --- |
| **P0** | Connect `readline -> lex -> parse -> expand -> execute` | `echo hello`, `pwd`, and `/bin/echo hello` run inside `./minishell` |
| **P0** | Implement `cd`, `export`, `exit` | Parent-shell state and exit cases match Bash/subject behavior |
| **P1** | Exercise and repair executor/pipes/path/redirection | Single and multi-command tests pass, correct statuses, no hangs |
| **P1** | Validate heredocs and signal transitions | Quoted/unquoted delimiters, Ctrl-C, EOF all behave correctly |
| **P1** | Audit expansion edge cases | Quote handling, `$?`, empty variables, fields and redirection targets pass |
| **P1** | Establish executable tests and memory/FD checks | Regression suite, Norminette, sanitizer/Valgrind and FD reports reviewed |
| **P2** | Final error handling, evaluation rehearsal, documentation sync | Clean build and reproducible test evidence for both partners |

**Do not calculate an updated completion percentage from file counts.** Working source lines and end-to-end shell correctness are different metrics.

---

## 2. 42 rules and team conventions

Use the applicable current 42 subject and Norminette as the final authority. Team rules for this repository:

- **Maximum 5 functions per `.c` file.**
- **Maximum 25 lines per function.**
- **Maximum 4 parameters per function.**
- **No ternary expressions.**
- Compile with `-Wall -Wextra -Werror`.
- Prefer `#include "minishell.h"` in `.c` files (umbrella header style) rather than mixing many local headers.
- Follow Norminette naming, indentation, declaration placement, and permitted constructs.
- Use only APIs permitted by the actual Minishell subject. Validate the final allowed-function list rather than assuming every standard function is permitted.
- Avoid general global state. The present `g_signal` is intended only for signal notification.
- Do not casually rename shared fields, headers, structs, exported functions, or their return contracts.
- Check allocation failure, `errno`, exit status, `close()`, and `waitpid()` paths, not only happy paths.

**Shared-interface rule:** a change to `include/types.h`, `include/{env,lexer,parser,expander,builtins,executor,signals}.h`, or `include/minishell.h` requires a review of every dependent module.

---

## 3. How to build and launch

```bash
git clone https://github.com/RussellFSJ/minishell.git
cd minishell
git checkout master
make
./minishell
```

The current `Makefile`:

- Builds `libft/libft.a` and all `src/**/*.c` objects.
- Links `-lreadline`.
- Provides `all`, `clean`, `fclean`, and `re`.
- Does **not currently** define named unit-test targets.

```bash
make fclean
make
make re
make clean
```

Readline headers/libraries must be available on the chosen platform. The project should be tested in the 42 evaluation environment. Do not interpret a successful launch as proof that commands execute: **the currently checked `main.c` still discards the entered command.**

For a reproducible handover, record the exact revision used for every test:

```bash
git rev-parse HEAD
git status --short
```

---

## 4. Architecture and execution lifecycle

### Intended flow

```text
Persistent shell state (t_shell)
       |
       v
readline()  -> history / input ownership
       |
       v
lex(line, &error)         -> t_token linked list
       |
       v
parse(tokens, &error)     -> t_command linked list + t_redir lists
       |                       (parser duplicates required token strings)
       +--> free_tokens(tokens)
       |
       v
expand(shell, commands)   -> expanded argv + redirection targets
       |
       v
execute(shell, commands)
       |
       +--> heredoc_collect() before command execution
       |
       +--> single builtin: run in parent with saved/restored stdio
       |
       +--> otherwise pipeline_run(): pipe/fork/child_run()
       |                           |
       |                           +--> redirect_apply()
       |                           +--> builtin_dispatch() or
       |                                 path_resolve() -> env_to_array() -> execve()
       |
       +--> status_collect()/status_decode(), close heredoc FDs
       |
       v
shell.last_status          -> next command's $?
       |
       +--> free_commands(commands)
       |
       v
Next prompt, or exit when shell.should_exit != 0
```

### Lifetime boundaries

- **Across prompts:** `t_shell` and its environment live until shell exit.
- **One input line:** raw `line`, token list, command list, expanded argument strings and redirection targets.
- **One execution:** forked children, pipes, saved parent standard descriptors and heredoc descriptors.
- **On every error:** unwind only what has already been allocated/opened, and leave the prompt usable unless shell termination was requested.

### Example: `echo "$USER" | wc -c`

1. Lexer creates `WORD(echo)`, `WORD("$USER")`, `PIPE`, `WORD(wc)`, `WORD(-c)`.
2. Parser creates two commands, linked in pipeline order.
3. Expander resolves `$USER` in the first command, while respecting double quotes.
4. Executor arranges the pipe and forks appropriate children.
5. First child executes the builtin `echo`; second executes `wc` through PATH resolution.
6. Parent closes unused pipe ends, waits for **all** children, and records **the last pipeline command's** exit status.
7. All command-owned allocations are freed; shell environment persists.

This example is the **target behavior**, not a claim that the current executable has passed it.

---

## 5. Current module status

**Status definitions:** `Implemented (source)` = meaningful code exists; `Partial` = substantive logic exists but important behavior or integration remains; `Stub` = function does not implement its purpose; `Pending validation` = no current end-to-end proof.

| Component | Current status | What Russell should do |
| --- | --- | --- |
| `t_env`, environment API | Implemented (source) | Verify NULL/unset/export semantics, allocation failures and conversion |
| Lexer | Implemented (source) | Regression-test quotes, operators, malformed input, empty words |
| Parser | Implemented (source) | Verify syntax errors, pipelines, redirections and ownership |
| Expander | Implemented (source), **needs audit** | Check quoting, field splitting, empty values, ambiguous redirects |
| `is_builtin()`, `builtin_dispatch()` | Implemented (source) | Verify null argv and all dispatch paths |
| `echo`, `pwd`, `env`, `unset` | Core implemented | Test options/identifiers, errors and parent state |
| `cd` | **Stub** | Implement and test `PWD`/`OLDPWD` semantics |
| `export` | **Stub** | Implement validation, assignment, export flag and listing |
| `exit` | **Stub** | Implement numeric parsing, overflow and `should_exit` |
| `execute()` | Implemented (source), **not connected** | Integrate, test parent-builtin redirections and error paths |
| PATH resolution | Implemented (source) | Test unset/empty PATH, relative paths, permission/status mappings |
| Pipes and forking | Implemented (source) | Verify EOF/FD closure, partial-fork failure, all waits |
| Child execution | Implemented (source) | Verify exec failures, diagnostics and exit statuses |
| Redirection application | Implemented (source) | Verify precedence, permissions, FD cleanup and no-command redirects |
| Heredoc collection | Implemented (source) | Review helper behavior, interrupt/EOF and FD lifecycle |
| Prompt/child/wait/heredoc signals | Implemented (source) | Audit async-signal-safe behavior and interactive results |
| `main.c` | **Not integrated** | Process lines, propagate status, free every stage |
| Current automated test targets | **Absent from root Makefile** | Create a repeatable regression workflow |
| Norminette / memory / FD / Bash comparison | **Pending validation** | Record results against exact commit |

The old README's statements that the entire executor, pipes, PATH, redirects, heredocs, and signals are stubs are **outdated**. Conversely, code appearing in those modules is not proof that the end-to-end project is finished.

---

## 6. Repository map and public function contracts

### Confirmed key paths

```text
minishell/
├── Makefile
├── README.md
├── include/
│   ├── minishell.h
│   ├── types.h
│   ├── env.h
│   ├── lexer.h
│   ├── parser.h
│   ├── expander.h
│   ├── builtins.h
│   ├── executor.h
│   └── signals.h
├── libft/
└── src/
    ├── main.c
    ├── builtins/
    │   ├── builtin_dispatch.c
    │   ├── builtin_cd.c
    │   ├── builtin_export.c
    │   └── builtin_exit.c
    ├── parser/parser.c
    ├── expander/
    │   ├── expander.c
    │   └── expander_variable.c
    ├── executor/
    │   ├── executor.c
    │   ├── executor_child.c
    │   ├── executor_pipes.c
    │   ├── executor_path.c
    │   └── executor_status.c
    ├── redirect/
    │   ├── redirect.c
    │   └── heredoc.c
    ├── signals/
    │   ├── signals.c
    │   └── signals_heredoc.c
    └── cleanup/free_commands.c
```

This is a **confirmed key-file map, not an exhaustive file tree**. Other lexer/parser/environment/expander/redirect helpers are part of the project; inspect `find src -type f -name '*.c' | sort` in the checkout to get the exact up-to-date list.

### Environment and cleanup APIs

| Function | Input / result | Called by / purpose | Error or ownership rule |
| --- | --- | --- | --- |
| `env_init(char **envp)` | Process environment -> `t_env *` | Shell startup | Result is shell-owned; distinguish failure from intentionally empty environment |
| `env_new(key, value, exported)` | Values -> node | Environment list construction | Allocate node and owned strings; free on failed insertion |
| `env_add_back(&env, node)` | Append node; `void` | Environment mutation | Do not pass an already-linked/freed node |
| `env_get(env, key)` | `char *` value or `NULL` | Expander, PATH, `cd` | **Borrowed pointer**: never free or modify directly |
| `env_set(&env, key, value)` | `int` status | `export`, `cd`, initialization | Check failure before declaring a state change successful |
| `env_unset(&env, key)` | `int` status | `unset` | Remove owned node safely; retain list integrity |
| `env_to_array(env)` | `char **` | Before external `execve()` | Newly allocated; caller frees on exec failure |
| `free_env(env)` | `void` | Final shell teardown | Use once after all consumers finish |
| `free_tokens(tokens)` | `void` | After parsing / failure | Owns lexer token nodes and values |
| `free_commands(cmds)` | `void` | After each input's execution/error | Frees argv, redirection nodes/strings, command nodes |
| `free_redirs(redirs)` | `void` | Within command cleanup | **Does not close FDs in current source**; runtime must close first |

### Frontend APIs

| Function | Output / return | Used by | Success condition / failure impact |
| --- | --- | --- | --- |
| `lex(line, &error)` | `t_token *` | Main input pipeline | Token list valid; on failure do not call parser |
| `lex_word(...)`, `lex_operator(...)`, `quote_scan(...)` | helper status | Lexer | Preserve token/quote contract; propagate syntax/allocation errors |
| `parse(tokens, &error)` | `t_command *` | Main input pipeline | Command list owns duplicates; on failure clean tokens and partial commands |
| `parser_check_syntax(tokens)` | status | Parser | Reject unsupported/incomplete operator placement |
| `parser_build_argv(...)`, `parser_add_redir(...)` | status | Parser | Build command-owned argv/redir structures |
| `expand(shell, cmds)` | `0` / `-1` | After parse, before execute | Mutates command-owned strings; on failure free entire command list |
| `expand_word(shell, word)` | allocated `char *` / `NULL` | Args/normal redir targets | Caller owns result; original word remains until successfully replaced |
| `expand_variable(shell, word, &pos)` | allocated replacement / `NULL` | Word expansion | Advances `pos`; caller must not increment it again for same `$` |
| `expand_argv(shell, argv)` | status | `expand()` | Must handle variable-derived empty or multi-field words correctly |
| `expand_redirs(shell, redir)` | status | `expand()` | Heredoc delimiter quote removal differs from filename expansion |
| `strip_quotes(word)` | allocated string / `NULL` | Heredoc delimiter handling | Only remove syntactic quotes from original input |

### Builtin and execution APIs

| Function | Return / effect | Called by | Required behavior and failure impact |
| --- | --- | --- | --- |
| `is_builtin(name)` | Boolean-style `int` | Executor | Null-safe; recognize seven mandatory builtins |
| `builtin_dispatch(shell, cmd)` | Builtin status, `-1` for invalid dispatch | Parent/child execution | Only dispatch known names; preserve/modify shell state appropriately |
| `builtin_echo/pwd/env/unset` | Command status | Dispatcher | Compare edge cases and diagnostics against Bash |
| `builtin_cd/export/exit` | Command status | Dispatcher | **Currently stubs**; must implement real behavior |
| `execute(shell, cmds)` | Command status | **To be called from main** | Collect heredocs, choose parent vs pipeline path, close heredocs, update `last_status` |
| `pipeline_run(shell, cmds)` | Pipeline status | `execute()` | Spawn/wait all children; last command determines status |
| `child_run(shell, cmd)` | No normal return | Pipeline child | Apply redirection; builtin or `execve()`; terminate on failure |
| `path_resolve(env, name)` | Allocated path / `NULL` | Child execution | Distinguish lookup/permission/memory failures; caller owns path |
| `status_collect(shell, last_pid)` | Decoded status or `-1` | Pipeline wait | Wait for specified child; caller must reap remaining children |
| `status_decode(wait_status)` | Shell exit status | `status_collect()` | Exit status or `128 + signal` convention |
| `redirect_apply(cmd)` | `0` success / nonzero error | Child and parent builtin | Apply ordered redirections and handle failed open/dup2 |
| `heredoc_collect(shell, cmds)` | `0` success / failure status | `execute()` | Prepare all heredocs before launching commands |
| `heredoc_run(...)`, `heredoc_read(...)` | Status | Heredoc internals | Verify delimiter, body expansion, EOF and interruption in source |
| `signals_setup_prompt/child/wait/heredoc()` | `void` | Readline/executor/heredoc | Install context-appropriate signal behavior |

**Contract change checklist:** If a helper's return convention changes, update its declaration, callers, cleanup behavior, tests and this document together. A status of `0` must not silently mean both “success” and “nothing was done” for mandatory builtins.

---

## 7. Shared structures, ownership, and invariants

The following structures are defined in `include/types.h`. These are **live integration contracts**.

```c
typedef enum e_token_type
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,
    TOKEN_REDIR_OUT,
    TOKEN_REDIR_APPEND,
    TOKEN_HEREDOC
}   t_token_type;

typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

typedef struct s_redir
{
    t_token_type    type;
    char            *target;
    int             expand_body;
    int             fd;
    struct s_redir  *next;
}   t_redir;

typedef struct s_command
{
    char                **argv;
    t_redir             *redirs;
    struct s_command    *next;
}   t_command;

typedef struct s_env
{
    char            *key;
    char            *value;
    int             exported;
    struct s_env    *next;
}   t_env;

typedef struct s_shell
{
    t_env   *env;
    int     last_status;
    int     should_exit;
}   t_shell;
```

### Invariants to keep

- A `t_token` owns its `value`; `free_tokens()` releases both. The parser copies data needed by commands before tokens are released.
- A `t_command` owns its `argv`, redirections and following command link. Parser and expander use these same command nodes.
- **`t_redir` is ordered.** Preserve left-to-right redirection order even if later redirections replace earlier ones.
- `t_redir.expand_body`: `1` means the heredoc delimiter was unquoted and the heredoc body permits variable expansion; `0` means a quoted delimiter and literal body. The delimiter itself is not variable-expanded.
- `t_redir.fd` starts at **`-1`**, becomes an open readable descriptor for a prepared heredoc, and must be closed once and set back to `-1`. `free_redirs()` currently does not close it; `execute()` has a separate close routine. Any early-abort path must be audited.
- `t_env.value` may be `NULL` for a name exported without a value. Preserve the distinction between unset, null-valued, and empty-valued variables where required.
- `env_get()` returns a **borrowed** pointer. `expand_variable()` duplicates environment values so command cleanup never frees persistent environment storage.
- `env_to_array()` returns a newly allocated `char **` appropriate for `execve()`. On `execve()` failure, free the array and its strings.
- `t_shell.env`, `last_status`, and `should_exit` persist between prompt iterations. Per-line token and command lists must not persist.
- `last_status` must represent the result of the latest foreground command/pipeline. Its value is used by later `$?` expansion.

### Ownership cheat sheet

| Resource | Created by | Released by |
| --- | --- | --- |
| `readline()` line | Readline | Main loop after processing |
| Token nodes and values | Lexer | `free_tokens()` |
| Command nodes and parser argv | Parser | `free_commands()` (or expander replaces individual argv strings) |
| Expanded argv and targets | Expander | `free_commands()` |
| Environment list | `env_init`, `env_set` | `free_env()` on shutdown |
| `env_get()` value | Environment | **Do not free (borrowed)** |
| `env_to_array()` | Environment helper | Executing child after `execve()` failure |
| `path_resolve()` result | Executor | Child after `execve()` failure |
| Pipe ends | `pipe()` | Both parent and children, according to FD ownership |
| Saved stdin/stdout | `dup()` in parent builtin | Restore using `dup2()`; then `close()` both |
| Heredoc descriptors | Heredoc collection | Executor close path, including failed collection |
| Spawned child processes | `fork()` | Parent `waitpid()` for every spawned PID |

---

## 8. Design choices that must be preserved

### Lexer preserves quotes until expansion

The lexer checks quote boundaries but does not strip quotation characters before the parser/expander can interpret them. These must remain different:

```bash
echo '$USER'   # literal
echo "$USER"   # expanded
```

**Reason:** stripping quotes early loses syntactic context. Quotation characters inside the *value* of a variable are data; they are not newly active quote syntax.

### Parser duplicates token strings

This permits:

```c
tokens = lex(line, &error);
cmds = parse(tokens, &error);
free_tokens(tokens);
```

**Reason:** executor inputs must remain valid after token cleanup. Do not replace duplication with token-pointer borrowing without redesigning lifetimes.

### Expansion uses `t_expand_ctx`

`word`, `result`, `i`, and `state` belong to one expansion operation. Grouping them keeps helper signatures within the argument limit and avoids globals. The quote state is `0` (unquoted), `1` (single), `2` (double).

**Important current-source correction:** `process_word_char()` currently **returns directly** after `expand_dollar()`, whose helper advances the index. The earlier concern about an unconditional second index increment is **not present in the reviewed version**. Preserve that behavior and test `$HOME/file`, `$USER.txt`, `$?`, trailing `$`, and quoted variants.

### Parent vs child builtin execution

A stand-alone builtin should execute in the **parent** so `cd`, `export`, `unset`, and `exit` can change persistent state. In a pipeline, the builtin executes in a **child** and does not change the parent environment or cwd.

For parent-run builtins with redirection (for example, `export X=1 > out`), save stdin/stdout, apply redirections, dispatch the builtin, **restore** the originals even on failure, then close the saved copies. A missing restore can redirect the next prompt into a file.

### Heredoc collection precedes pipeline launch

Prepare heredoc bodies before spawning commands. Preserve quoted delimiter behavior and hand off readable descriptors to redirection execution. On Ctrl-C, abort the pending command line without killing the interactive shell.

### Wait for all children, report the last command's status

`false | true` should normally yield status `0`; `true | false` should yield `1`. **Last in pipeline** does not mean *last process to finish*. Reap every PID to avoid zombies.

---

## 9. Known blockers and review risks

### Blockers

**B1. Main-loop integration is missing (P0).**  
`src/main.c` does not process input through the frontend or executor. Nothing else can receive a credible end-to-end “pass” until this is connected.

**B2. Three mandatory builtins are placeholders (P0).**  
`src/builtins/builtin_cd.c`, `builtin_export.c`, and `builtin_exit.c` return `0` without changing state, printing expected output, or terminating when appropriate.

**B3. Previous README test commands are stale (P1).**  
The current root Makefile has no `tests` or `test_*` targets. Previous parser/expander/builtin Valgrind statistics were **historical unit-test reports**, not a verification of today's integrated executable.

### Audit findings: not yet proved runtime failures

| Area | What to inspect | Example regression |
| --- | --- | --- |
| Expander field behavior | `expand_argv()` appears to replace individual strings; confirm correct unquoted word splitting/empty-field behavior | `X='a b'; ...` with quoted vs unquoted `$X` |
| Redirection expansion | Detect ambiguous targets created by unquoted expansion; do not treat every expansion as one literal filename | `X='a b'; echo hi > $X` |
| Signal-safety | Current prompt SIGINT handler calls Readline functions; verify permitted and async-signal-safe approach in target environment | Ctrl-C during Readline, repeated rapidly |
| Heredoc interruption | Check partial file/descriptors after Ctrl-C and EOF | `cat << EOF`, Ctrl-C; next command |
| PATH lookup | Inspect `errno`, missing PATH, empty PATH components and directory handling | command not found / permission denied / is directory |
| Pipeline failure | On partial `pipe()`/`fork()` failure, close every FD, kill/reap spawned children and return nonzero | Stress many pipeline stages / low FD limit |
| Bare redirects | A command with redirections but no `argv[0]` must still perform filesystem side effects | `> file`, `< missing_file` |
| FD ownership | `free_redirs()` does not close saved `redir->fd`; ensure all routes reach a close | interrupt/failed heredoc and next prompt |
| 42 Norm | A few execution helpers are long enough to warrant a Norminette check, including `parent_builtin()` and `pipeline_wait()` | `norminette src include` |
| Exit/status output | Diagnostic text and 126/127 classification must match required behavior | `./missing`, directory, nonexecutable file |

These are **verification tasks**, not claims that each sample currently fails.

---

## 10. Ordered completion plan

Follow this order because each step unlocks a useful test for the next. Keep commits small and testable.

### Phase 0: Establish a truthful baseline

- [ ] Pull latest `master`, capture `git rev-parse HEAD`, and ensure clean working tree.
- [ ] Run `make fclean && make`, log warnings/errors.
- [ ] Run `norminette src include`; note any existing failures separately from newly introduced failures.
- [ ] Inventory files and functions: `find src -type f -name '*.c' | sort`.
- [ ] Confirm which existing tests still exist and how they are run; remove outdated README references only after deciding the new test workflow.

**Gate:** one recorded baseline, no guessing from the August README.

### Phase 1: Wire the shell's front-to-back path

- [ ] Add a per-line processing helper invoked by `read_one_line()` in `main.c`.
- [ ] Lex input and handle syntax/quote errors without executing invalid commands.
- [ ] Parse tokens; free tokens once their values have been duplicated.
- [ ] Expand argv and redirection targets; stop execution on expansion failure.
- [ ] Call `execute(shell, cmds)` on the prepared command list.
- [ ] Free commands and line memory on every successful and failed path.
- [ ] Return consistent exit statuses for empty input, syntax errors and allocation errors.
- [ ] Keep signal setup and `g_signal` transitions coherent.

**Gate:** `echo hello`, `pwd`, `/bin/echo hello`, `false`, `echo $?`, and two sequential commands work interactively.

### Phase 2: Finish the remaining builtins

- [ ] Implement `cd` with HOME, relative/absolute paths, error handling, and appropriate PWD/OLDPWD updates.
- [ ] Implement `export` with identifier validation, assignment, bare declaration and no-argument listing.
- [ ] Implement `exit` with numeric argument parsing, overflow, modulo-256 result and too-many-arguments rules.
- [ ] Verify `unset` and export flags interact correctly; verify empty vs unset values.
- [ ] Check parent-run builtins with `<`, `>`, `>>`, and failed redirection.

**Gate:** state changes survive for subsequent commands, but do not leak out of pipeline children.

### Phase 3: Harden external execution and pipelines

- [ ] Test explicit executable path, PATH lookup, relative path and unset/empty PATH.
- [ ] Ensure 126/127 and diagnostic selection follow Bash/subject requirements.
- [ ] Check every `fork()`, `pipe()`, `dup2()`, `execve()`, and `waitpid()` failure path.
- [ ] Verify no pipe end stays open in the parent or unrelated children.
- [ ] Wait for every spawned PID and return the final stage's status.
- [ ] Test builtin/external mixes and commands that contain only redirects.

**Gate:** pipelines do not hang or leak descriptors; status matches Bash for supported input.

### Phase 4: Harden redirections, heredocs and signals

- [ ] Confirm redirections are executed left-to-right, including mixed input/output combinations.
- [ ] Verify `0666` creation mode is filtered by process `umask`; failed opens prevent command execution.
- [ ] Confirm heredoc delimiter quotes are removed without expanding the delimiter itself.
- [ ] Confirm body variable expansion depends on `expand_body`.
- [ ] Validate multiple heredocs, end-of-file before delimiter and Ctrl-C cancellation.
- [ ] Verify `g_signal` is restricted to signal notification and that handler operations follow the applicable subject and platform rules.
- [ ] Restore interactive signal handlers after child wait/heredoc paths.

**Gate:** interactive prompts remain usable after normal execution, errors, Ctrl-C and Ctrl-D.

### Phase 5: Expansion and parser correctness

- [ ] Test single, double and adjacent quotes, empty quotes, dollar signs and variable characters.
- [ ] Test empty and multiword expansions in command arguments.
- [ ] Test ambiguous redirect expansion cases against Bash within the Minishell scope.
- [ ] Test syntax errors before pipes, after pipes and after redirects.
- [ ] Re-run lexer/parser/expander memory checks with realistic error inputs.

**Gate:** frontend and executor agree on the number and content of arguments and redirections.

### Phase 6: Submission hardening

- [ ] Clean compilation with `-Wall -Wextra -Werror`.
- [ ] Norminette passes, including headers and newly split helpers.
- [ ] Automated behavior comparisons pass against Bash for required features.
- [ ] No project-owned reachable leaks, invalid memory operations, double frees or leaked file descriptors on tested paths.
- [ ] `make`, `clean`, `fclean`, and `re` work as expected.
- [ ] README reflects the final actual implementation and both partners have rehearsed evaluation questions.

**Gate:** every acceptance checkbox in Section 16 is supported by recorded test evidence.

---

## 11. Builtins: requirements and acceptance tests

| Builtin | Required behavior | Common mistakes |
| --- | --- | --- |
| `echo` | Print arguments, support `-n` and repeated `n` flags where expected | Misinterpreting `-nnn`, extra/missing newline |
| `pwd` | Print current working directory | Stale PWD variable, unexpected error handling |
| `env` | Show exported environment entries with defined values | Printing unexported or null-value entries |
| `unset` | Remove identifiers without deleting unrelated env nodes | Incorrect linked-list relinking; bad identifier behavior |
| `cd` | Change directory in parent, report errors; maintain PWD/OLDPWD as appropriate | Forking for standalone builtin; updating vars on failure |
| `export` | Validate names; set/export vars; list exported names with no args | Losing distinction between `export X` and `export X=` |
| `exit` | Exit current shell for valid cases; set exit code correctly; handle invalid/multiple args | Treating every bad argument the same; terminating when too many args |

### Focused interactive test sequence

```bash
pwd
cd /tmp
pwd
cd /
pwd
cd /this/path/does/not/exist
pwd
export TEST=hello
echo "$TEST"
env
unset TEST
echo "$TEST"
export EMPTY=
export DECLARED
export
exit 42
```

Run a **separate Minishell instance** for each `exit` edge case:

```bash
exit
exit 0
exit -1
exit 256
exit 1 2
exit abc
exit 999999999999999999999999999
```

Validate output **and process return code** (`echo $?` in the launching shell after Minishell exits). A printed error followed by return code `0` is not a passing result.

---

## 12. Execution, redirection, heredoc, and signals

### Runtime decision table

| Command form | Execution location | Persistent parent state? | Examples |
| --- | --- | --- | --- |
| One builtin | Parent | Yes, if builtin mutates state | `cd /tmp`, `export X=1` |
| One external | Forked child | No | `/bin/ls`, `grep` |
| Any pipeline | One child per stage | No state changes survive from pipeline children | `export X=1 | cat` |
| Redirect-only command | Filesystem effects must occur | No variable state needed | `> out`, `< missing` |

### Paths and statuses

- A name containing `/` is an explicit path candidate. A slashless name normally searches directories in `PATH`.
- `execve()` replaces a successful child process and **never returns on success**.
- On failure, report an appropriate diagnostic, clean child-owned memory, and terminate with the correct status.
- Typical shell conventions: **127** = command not found; **126** = found but cannot execute; **128 + signal** = signal termination. Confirm exact scope against evaluation requirements.
- A successful pipeline's status comes from the **last pipeline stage**.

### File descriptors and ordering

| Syntax | Operation | Target descriptor |
| --- | --- | --- |
| `< file` | `open(file, O_RDONLY)` | stdin (0) |
| `> file` | `open(file, O_WRONLY | O_CREAT | O_TRUNC, 0666)` | stdout (1) |
| `>> file` | `open(file, O_WRONLY | O_CREAT | O_APPEND, 0666)` | stdout (1) |
| `<< DELIM` | Readable prepared heredoc FD | stdin (0) |

Apply redirections **in source order**: `echo hi > a > b` truncates/creates `a` as a side effect, then directs final output to `b`. For child pipelines, connect the pipe with `dup2()` before applying explicit command redirections; an explicit redirection may override the inherited pipe endpoint.

Never leave an unused pipe write end open in the parent. Readers such as `cat` can wait indefinitely for an EOF that never arrives.

### Heredoc contract

```bash
cat << EOF
$HOME
EOF

cat << 'EOF'
$HOME
EOF
```

The first permits body expansion; the second reads `$HOME` literally. Quoting changes expansion of the **body**, not the delimiter text that terminates the heredoc. A cancelled heredoc should prevent the associated pipeline from executing and should leave the next prompt usable.

### Signal behavior matrix

| Context | Ctrl-C (SIGINT) | Ctrl-\ (SIGQUIT) | Ctrl-D |
| --- | --- | --- | --- |
| Interactive empty prompt | New prompt / status 130 | Ignored | Readline returns `NULL`; clean exit |
| Foreground child | Child handles default SIGINT | Child handles normal SIGQUIT | Program-specific input EOF |
| Parent while waiting | Must not be killed by terminal signal | Must not be killed by terminal signal | Not a separate signal |
| Heredoc collection | Abort current heredoc/command | Usually ignored | Warn or finish per applicable behavior; no infinite loop |

Current source has `signals_setup_prompt()`, `signals_setup_child()`, `signals_setup_wait()`, and `signals_setup_heredoc()`. **Audit the prompt handler's Readline calls for async-signal-safety** before considering the signal subsystem finished.

---

## 13. Testing and memory/FD validation

### Minimal manual Bash comparison matrix

Run supported command lines **in both Bash and Minishell**, comparing stdout, stderr, side effects, status and whether the shell remains interactive. Because `cd`, `export`, and `exit` affect process state, use fresh shells and comparable environments when needed.

| Test group | Commands |
| --- | --- |
| Basic | `echo hi`; `pwd`; `/bin/echo hi`; `nosuchcommand_42` |
| Quotes | `echo '$HOME'`; `echo "$HOME"`; `echo ""`; `echo ''`; `echo a"b"c` |
| Expansion | `echo $?`; `echo "$NOT_SET"`; `echo $HOME/file`; `echo $USER.txt`; `echo '$?'` |
| Pipelines | `echo hi \| cat`; `true \| false`; `false \| true`; `printf hello \| wc -c` |
| Redirections | `echo hi > out`; `echo again >> out`; `cat < out`; `> empty`; `cat < missing` |
| Ordering | `echo hi > a > b`; `cat < a < b`; `echo hi > /no/such/dir/out` |
| Stateful builtin | `cd /tmp` then `pwd`; `export X=1` then `echo "$X"`; `unset X` |
| Heredoc | `cat << EOF`; `cat << 'EOF'`; two heredocs in one command; Ctrl-C midway |
| Failures | Unmatched `'`/`"`; leading/trailing `\|`; `cat >`; no PATH; execute directory |
| Signals | Ctrl-C/Ctrl-\ at idle prompt; while running `cat`; inside a heredoc |

The `\|` in the Markdown table is a table escape; type a normal `|` when running those commands. Use a **fresh temporary directory** for file-creation tests.

### Smoke tests after main integration

```bash
make fclean && make
./minishell
```

Inside Minishell:

```bash
echo hello
/bin/echo hello
pwd
false
echo $?
echo hello | cat
exit
```

Expected: actual output from commands; no hang; prompt returns appropriately; no silent placeholder success.

### Norminette

```bash
norminette src include
```

Check function lengths and function counts after splitting implementation helpers. **Do not 'fix' Norm by silently changing function behavior.**

### Valgrind (Linux/evaluation environment)

```bash
valgrind \
  --leak-check=full \
  --show-leak-kinds=all \
  --track-origins=yes \
  --track-fds=yes \
  --errors-for-leak-kinds=all \
  --error-exitcode=42 \
  ./minishell
```

Test at least normal command execution, pipelines, malformed input, redirect errors, heredoc interrupts, and exit. Record:

- invalid reads/writes, invalid frees, double frees;
- definitely/indirectly lost allocations owned by the project;
- open FDs remaining unexpectedly after a command and at shutdown;
- unreaped child processes and hung pipelines.

Readline may retain library-owned allocations depending on platform/build. Investigate them separately rather than treating **all** leaks as automatically acceptable or suppressing project-owned leaks.

For platforms where Valgrind is unavailable or unreliable, use AddressSanitizer/LeakSanitizer where supported, then run final Valgrind on the evaluation-compatible Linux system. **The historical August unit-test “0 leaks” figures do not validate the October integrated shell.**

### Regression-test workflow

The current `Makefile` has no `make tests` target. Recommended next step: add an explicit `test` or `tests` target that **runs** tests, returns nonzero on failure, and captures a short summary. Until then, do not document `make tests` as a working command. Avoid committing generated binaries or temporary test outputs.

---

## 14. Review checklist and collaboration protocol

### Before changing a public contract

1. Identify callers using `grep -R` or `rg`.
2. Document which inputs and return codes currently mean success/failure.
3. Check ownership before modifying allocations or FD lifecycle.
4. Update both declaration and implementation together.
5. Add regression tests for the old and new behavior.
6. Request partner review for changes to `types.h` or shared headers.

### Before each commit

- [ ] `make` succeeds with strict warnings.
- [ ] Norminette passes for touched C/header files (or existing failures are separately documented).
- [ ] Relevant functionality was exercised through the **real Minishell**, if integrated.
- [ ] Each new allocation and FD has a release path on success and failure.
- [ ] Function contracts and ownership are unchanged or deliberately updated.
- [ ] No accidental generated files, build artifacts or secrets are staged.
- [ ] Test result and any remaining limitations are mentioned in the commit/PR description.

Suggested small commit sequence:

```text
feat(main): connect lexer parser expander and executor
feat(builtins): implement cd and directory state updates
feat(builtins): implement export identifier and env handling
feat(builtins): implement exit status parsing and termination
fix(executor): harden pipeline cleanup and status handling
fix(redirect): cover ordering and fd failure paths
fix(heredoc): handle interrupts and quoted delimiters
fix(expander): handle empty fields and redirect edge cases
test(minishell): add bash comparison and memory regression suite
docs(readme): update completion evidence and handover status
```

These are **suggested future messages**, not claims that these changes have been made.

### Evidence log template

Copy this into a PR description or handover note as changes land:

```text
Commit SHA:
Feature / changed files:
Commands tested:
Expected stdout/stderr/status:
Actual stdout/stderr/status:
Norminette result:
Valgrind / sanitizer result:
FD / child-wait result:
Known limitations:
Reviewed by:
```

---

## 15. Evaluation questions

Both partners should be able to explain not only **what** the functions do, but **why** the implementation is designed this way:

1. Why must quote characters survive lexing until expansion?
2. Why does the parser copy data instead of keeping pointers into tokens?
3. Who frees `argv[i]` before and after expansion?
4. Why must `env_get()` return a borrowed pointer while `expand_variable()` duplicates the value?
5. What does `expand_body` record and why is the heredoc delimiter itself not variable-expanded?
6. Why do parent-state builtins run in the parent for a single command, but in children in pipelines?
7. How do you restore stdin/stdout after a redirected parent builtin, even on failure?
8. Why are pipes created **before** forking and unused FDs closed in both parent and child?
9. What happens when the parent leaves the pipe write end open?
10. How can `false | true` return 0 even if `false` finishes last?
11. How does `status_decode()` handle signals and why is `$?` stored in `t_shell`?
12. What is the ownership of `path_resolve()` and `env_to_array()` results if `execve()` fails?
13. How do redirection order and command-side file creation interact?
14. How are empty expansions, quoted empty arguments and ambiguous redirections differentiated?
15. Why is an interrupted heredoc different from a regular EOF?
16. What may safely be done inside a signal handler?
17. What happens when parsing succeeds but expansion fails halfway through?
18. How do you guarantee all children are waited for after a partial `fork()` failure?
19. Which files/functions have shared contracts, and how do changes propagate across the project?
20. How was the final binary validated beyond “it compiled” or “a unit test printed expected output”?

---

## 16. Definition of done

Mark Minishell complete **only when all items below are satisfied with evidence for a specific commit**:

- [ ] Real input is processed through lexer, parser, expander and executor.
- [ ] All seven mandatory builtins work within the project's allowed scope.
- [ ] Standalone stateful builtins correctly change the parent shell.
- [ ] External commands work by explicit path and PATH lookup.
- [ ] Redirections `<`, `>`, `>>` work in order and on failure paths.
- [ ] Heredocs `<<` handle quoted delimiters, body expansion, EOF and interrupts.
- [ ] Pipelines execute, close unused FDs, reap every child and report last-stage status.
- [ ] `$?` and signal exit statuses match the required behavior.
- [ ] Ctrl-C, Ctrl-D and Ctrl-\ work appropriately at prompt, in foreground children and during heredocs.
- [ ] Syntax and expansion edge cases are tested without crashes or silent misexecution.
- [ ] No invalid frees, invalid reads/writes, project-owned memory leaks, FD leaks or zombie children remain on tested paths.
- [ ] `make`, `clean`, `fclean` and `re` work, strict compilation flags pass, and Norminette passes.
- [ ] Bash-comparison tests pass for the mandatory Minishell scope.
- [ ] Both partners can explain structures, call graph, ownership, return codes, failure paths, and design tradeoffs.
- [ ] The README and recorded completion evidence match the actual submitted commit.

### Final handover instruction

**Preserve what is already built; integrate and test it before rewriting it.** The immediate blockers are the missing call chain in `main.c` and the three placeholder builtins. The execution layer has progressed beyond the previous README's stub descriptions and should be **validated and repaired**, not restarted from zero.

**This file is a guide to completing the project, not a declaration that the project is complete.**

