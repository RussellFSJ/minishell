# 42 Minishell — Project Handover & Completion Guide

> **Current internal progress estimate:** ~67% complete as of 30 August 2026.
> This percentage is a project-management estimate, not a 42 grading metric.

This document is the working handover reference for finishing the Minishell project. It explains what is already implemented, what must remain stable, what is still incomplete, how the modules depend on one another, and the reasoning behind several important design decisions.

The most important handover principle is simple:

> **The frontend is now substantially built. The remaining work should focus on runtime execution, the remaining stateful builtins, redirections/heredoc execution, signals, and final integration.**

---

## 1. Project Rules

Keep these constraints in mind for every new change:

* Maximum **5 functions per `.c` file**.
* Maximum **25 lines per function**.
* Maximum **4 function arguments**.
* **No ternary operators**.
* Prefer `#include "minishell.h"` in source files where practical.
* Compile with `-Wall -Wextra -Werror`.
* Run Norminette before committing.
* Check allocations and execution paths with Valgrind.
* Do not change shared structs or public function contracts casually. Several modules already depend on them.

---

## 2. Current Architecture

The intended command flow is:

```text
readline()
    |
    v
LEXER
    |
    | t_token list
    v
PARSER
    |
    | t_command list
    v
EXPANDER
    |
    | expanded argv/redirection targets
    v
HEREDOC COLLECTION
    |
    v
EXECUTOR
    |
    +--> BUILTIN
    |
    +--> PATH RESOLUTION --> execve()
    |
    +--> REDIRECTIONS
    |
    +--> PIPELINES
    v
STATUS COLLECTION
    |
    v
shell.last_status
```

The first three major transformation stages are already implemented and tested:

```text
input -> lexer -> parser -> expander
```

The main missing piece is turning the resulting `t_command` list into real running processes.

---

## 3. Current Completion Status

| Module                 | Status             | Notes                                                             |
| ---------------------- | ------------------ | ----------------------------------------------------------------- |
| Environment            | ✅ Implemented      | Linked-list environment storage and conversion                    |
| Cleanup                | ✅ Implemented      | Tokens, commands/redirections, environment                        |
| Lexer                  | ✅ Implemented      | Words, quotes, pipes and redirection operators                    |
| Parser                 | ✅ Implemented      | Syntax validation, argv construction, redirection list, pipelines |
| Expander               | ✅ Implemented      | Quote state, `$VAR`, `$?`, argv and redirect expansion            |
| `is_builtin()`         | ✅ Implemented      | Recognises all seven required builtins                            |
| `builtin_dispatch()`   | ✅ Implemented      | Explicit dispatch; unknown names return `-1`                      |
| `echo`                 | ✅ Core implemented | Supports repeated `-n` forms such as `-nnn`                       |
| `pwd`                  | ✅ Core implemented | Uses `getcwd()`                                                   |
| `env`                  | ✅ Core implemented | Prints exported variables with values                             |
| `unset`                | ✅ Core implemented | Removes variables through `env_unset()`                           |
| `cd`                   | ⏳ Stub             | Russell to implement                                              |
| `export`               | ⏳ Stub             | Russell to implement                                              |
| `exit`                 | ⏳ Stub             | Russell to implement                                              |
| Executor               | ⏳ Stub             | Main remaining runtime work                                       |
| PATH resolution        | ⏳ Stub             | Required for external commands                                    |
| Pipelines              | ⏳ Stub             | `pipe`, `fork`, `dup2`, close discipline                          |
| Redirection execution  | ⏳ Stub             | `<`, `>`, `>>`, heredoc fd                                        |
| Heredoc runtime        | ⏳ Stub             | Delimiter/body handling and signals                               |
| Signals                | ⏳ Stub             | Prompt, child and heredoc modes                                   |
| Main shell integration | ⏳ Incomplete       | Current main loop only reads/frees input                          |
| Final Bash comparison  | ⏳ Pending          | Required before submission                                        |

---

## 4. Shared Data Structures — Do Not Change Casually

Defined in `include/types.h`.

### `t_token`

```c
typedef struct s_token
{
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;
```

Produced by the lexer and consumed by the parser.

**Ownership:** the token owns `value`. `free_tokens()` frees both the value and node.

The parser duplicates values that it needs, so the token list can safely be destroyed after parsing.

### `t_redir`

```c
typedef struct s_redir
{
    t_token_type    type;
    char            *target;
    int             expand_body;
    int             fd;
    struct s_redir  *next;
}   t_redir;
```

Important fields:

* `type`: `<`, `>`, `>>`, or `<<`.
* `target`: filename or heredoc delimiter.
* `expand_body`: meaningful for heredoc.
* `fd`: initialized to `-1`; intended for runtime/heredoc descriptor handling.

### `expand_body` contract

For:

```bash
cat << EOF
```

`expand_body = 1`.

For:

```bash
cat << "EOF"
```

`expand_body = 0`.

The expander strips the quotes from the delimiter itself, but **does not variable-expand the heredoc delimiter**.

Later, `heredoc_collect()` should use `expand_body` to decide whether `$VARIABLE` expansion is allowed inside the heredoc body.

Do not lose this flag during execution.

### `t_command`

```c
typedef struct s_command
{
    char                **argv;
    t_redir             *redirs;
    struct s_command    *next;
}   t_command;
```

Each node represents one command in pipeline order.

For:

```bash
echo hello | grep h | wc -l
```

conceptually:

```text
command 0 -> command 1 -> command 2 -> NULL
   echo        grep          wc
```

The executor should preserve this order when creating pipes and deciding which process supplies `last_status`.

### `t_env`

```c
typedef struct s_env
{
    char            *key;
    char            *value;
    int             exported;
    struct s_env    *next;
}   t_env;
```

This is the shell's authoritative environment representation.

External commands cannot receive this linked list directly. `env_to_array()` converts it to the `char **envp` form required by `execve()`.

### `t_shell`

```c
typedef struct s_shell
{
    t_env   *env;
    int     last_status;
    int     should_exit;
}   t_shell;
```

This is persistent shell state.

* `env` must survive from command to command.
* `last_status` feeds `$?` expansion.
* `should_exit` allows the `exit` builtin to terminate the main loop cleanly.

---

## 5. Environment Module

Public API:

```text
env_init()
env_new()
env_add_back()
env_get()
env_set()
env_unset()
env_to_array()
free_env()
```

### Important ownership rule: `env_get()`

`env_get()` returns the environment node's existing `value` pointer.

It is a **borrowed pointer**.

Do not free it.

This is why `expand_variable()` duplicates the returned value before returning its expansion result. If the expander returned the environment pointer directly, later cleanup of the expanded argv could free memory still owned by `t_env`.

### `env_to_array()`

Returns a newly allocated `char **` suitable for:

```c
execve(path, cmd->argv, envp);
```

The caller owns this array and all strings inside it and must free it if `execve()` does not replace the process.

---

## 6. Lexer

The lexer converts raw input into a linked list of tokens.

Supported token types:

```text
TOKEN_WORD
TOKEN_PIPE
TOKEN_REDIR_IN
TOKEN_REDIR_OUT
TOKEN_REDIR_APPEND
TOKEN_HEREDOC
```

Operators:

```text
|   -> TOKEN_PIPE
<   -> TOKEN_REDIR_IN
>   -> TOKEN_REDIR_OUT
>>  -> TOKEN_REDIR_APPEND
<<  -> TOKEN_HEREDOC
```

### Why quotes are preserved by the lexer

The lexer validates matching quotes but keeps quotation characters inside the word token.

Example:

```bash
echo "$USER"
```

The parser/expander still needs to know that `$USER` was double quoted.

If the lexer removed quotes too early, the expander could not distinguish:

```bash
"$USER"
'$USER'
```

Those two inputs have different shell semantics.

Therefore quote removal belongs later, during expansion.

---

## 7. Parser

The parser converts `t_token` into `t_command` + `t_redir` structures.

Main responsibilities:

* validate pipe/redirection syntax;
* create one `t_command` per pipeline segment;
* build `argv` while excluding redirection operators/targets;
* construct ordered redirection lists;
* preserve heredoc quote information in `expand_body`.

### Syntax checks currently handled

Examples rejected by the parser include:

```bash
| ls
ls |
ls || cat
cat >
cat <<
cat > | wc
```

### Ownership boundary

The parser duplicates token values into command-owned memory.

Therefore the intended pattern is:

```c
tokens = lex(line, &error);
cmds = parse(tokens, &error);
free_tokens(tokens);
```

`cmds` remains valid after the token list is freed.

---

## 8. Expander

The expander is complete at the current handover point.

Supported behaviour includes:

```bash
"$USER"              -> variable expands
'$USER'              -> remains literal
$?                   -> shell.last_status
$UNKNOWN             -> empty string
abc"$USER"           -> concatenated expansion
"$HOME/file"         -> expanded path
```

### Quote state

The expansion state uses:

```text
0 = outside quotes
1 = inside single quotes
2 = inside double quotes
```

Single quotes suppress variable expansion.

Double quotes allow variable expansion.

### Why `t_expand_ctx` exists

`t_expand_ctx` groups state belonging to one word expansion:

```text
word
result
i
state
```

This was chosen because:

1. these values represent one logical operation;
2. it avoids passing many unrelated-looking parameters;
3. it keeps helper signatures within the 42 argument limit;
4. it makes expansion state explicit rather than global.

### Why quote removal happens while processing the original word

Do not expand a variable and then blindly call a quote-stripper over the final result.

Suppose an environment variable itself contains quotation characters. Those quotation characters are data and must not suddenly become shell syntax.

The expander therefore interprets syntactic quotes from the original command while building the output.

### `expand_argv()` ownership

For every argv entry:

```text
old parser-owned string
        |
        v
expand_word()
        |
        v
new allocated string
```

Only after expansion succeeds is the old string freed and replaced.

### Redirection expansion

Normal redirect targets use `expand_word()`.

Heredoc delimiters only have syntactic quotes removed:

```bash
cat << "$USER"
```

The delimiter becomes literal:

```text
$USER
```

not the environment value.

---

## 9. Builtins Already Implemented

### `is_builtin()`

Recognises:

```text
echo
cd
pwd
export
unset
env
exit
```

### `builtin_dispatch()`

Dispatches only recognised names.

Unknown commands return `-1`; they must **not** fall through to `exit` or another builtin.

The executor should normally call `is_builtin()` before `builtin_dispatch()`.

### `echo`

Core behaviour implemented, including repeated `-n` forms:

```bash
echo -n hello
echo -nn hello
echo -nnnn hello
```

### `pwd`

Uses `getcwd(NULL, 0)` and frees the returned buffer.

### `env`

Prints nodes for which:

```text
exported == 1
and
value != NULL
```

### `unset`

Currently delegates removal to `env_unset()`.

Russell should still compare identifier/error behaviour against Bash during final builtin hardening.

---

## 10. Remaining Builtins

### `cd`

Must at minimum handle:

```text
cd path
cd            -> HOME
```

On successful directory change:

1. capture old working directory;
2. call `chdir()`;
3. determine the new working directory;
4. update `OLDPWD`;
5. update `PWD`.

Do not update environment state if `chdir()` fails.

Compare failure messages/statuses with Bash.

### `export`

Needs identifier validation.

Valid identifier shape:

```text
[first character] alphabetic or _
[remaining]       alphanumeric or _
```

Important cases:

```bash
export TEST=value
export TEST=
export TEST
export _TEST=1
export 1TEST=x
export A-B=x
```

`export` with no arguments must print exported variables in the expected format/order required by the subject/evaluation behaviour.

The existing environment API can be reused, but export-without-value may require careful handling of `value == NULL` and the `exported` flag.

### `exit`

Important cases:

```bash
exit
exit 42
exit -1
exit abc
exit 1 2
exit 999999999999999999999
```

Distinguish:

* numeric argument;
* numeric overflow;
* too many arguments.

The final shell status is normally constrained to an unsigned 8-bit exit code (`0..255`) after conversion.

`exit` must update `shell->should_exit` only when the shell should actually terminate.

---

## 11. Critical Executor Design Decision: Parent vs Child Builtins

Not every builtin can simply run in a forked child.

These modify persistent shell state:

```text
cd
export
unset
exit
```

For a **single non-pipeline command**, they generally need to execute in the parent shell so their changes survive.

Example:

```bash
cd /tmp
pwd
```

If `cd` runs only in a child, the parent remains in the original directory and the next `pwd` is wrong.

Likewise:

```bash
export X=hello
```

must modify the parent environment.

However, a builtin inside a pipeline should execute in a child context because pipeline components are separate processes:

```bash
export X=hello | cat
```

The executor therefore needs to distinguish:

```text
single command + builtin
        vs
pipeline command
```

This is one of the most important runtime design choices remaining.

---

## 12. Recommended Executor Implementation Order

### Step 1 — `path_resolve()`

Implement external command lookup first.

Rules:

1. If command name contains `/`, treat it as an explicit path.
2. Otherwise fetch `PATH` with `env_get()`.
3. Split `PATH` by `:`.
4. Join each directory with command name.
5. Test candidates using `access()`.
6. Return a newly allocated successful path.
7. Return `NULL` if unresolved.

Be precise later about the difference between:

```text
command not found
permission denied
is a directory
```

because they map to different diagnostics/statuses.

### Step 2 — `status_decode()` / `status_collect()`

Centralise wait-status interpretation.

The parent must update:

```c
shell->last_status
```

from the **last command in the pipeline**, not whichever child happens to finish last.

Also account for children terminated by signals.

### Step 3 — `redirect_apply()`

Process redirections **left to right**.

Example:

```bash
cmd > a > b
```

Final stdout must go to `b`.

Implement:

```text
<    open read-only -> dup2 to STDIN
>    create/truncate -> dup2 to STDOUT
>>   create/append -> dup2 to STDOUT
<<   use previously prepared heredoc fd -> dup2 to STDIN
```

Close descriptors after `dup2()` when no longer needed.

### Step 4 — `heredoc_collect()`

Collect heredocs before launching the pipeline.

For each heredoc redirection:

1. read until delimiter;
2. if `expand_body == 1`, expand variables in body lines;
3. if `expand_body == 0`, keep body text literal;
4. store a readable descriptor in `redir->fd`;
5. executor later redirects stdin from this fd.

Do not re-expand the delimiter. The expander has already handled its quote removal.

### Step 5 — `child_run()`

Child execution should roughly do:

```text
configure child signals
apply pipe descriptors
apply command redirections
if builtin:
    run builtin
    exit with returned status
else:
    resolve path
    convert env list using env_to_array()
    execve()
```

Remember: successful `execve()` never returns.

Any allocated path/environment array must be cleaned if execution fails.

### Step 6 — `pipeline_run()`

For N commands, create the necessary pipes and fork each command.

Key rules:

* child receives the appropriate previous read end as stdin;
* child receives next pipe write end as stdout when needed;
* both parent and children close descriptors they do not use;
* parent must not keep a pipe write end open accidentally, or readers may wait forever for EOF;
* remember the PID of the final command for final status selection.

Descriptor discipline is more important than making the loop look clever.

### Step 7 — `execute()`

`execute()` should be an orchestrator rather than a giant function.

Recommended decision flow:

```text
if no command:
    return

collect heredocs

if exactly one command and it is a builtin that must affect parent state:
    execute builtin in parent with temporary redirections
else:
    run pipeline/external execution path

collect/update status
```

Keep low-level pipe/path/redirection work in their respective files.

---

## 13. Parent Builtin Redirection Warning

Running a stateful builtin in the parent creates one extra problem:

```bash
export X=1 > file
```

The builtin needs parent state **and** redirection.

The safe pattern is:

```text
dup(STDIN)
dup(STDOUT)
    |
apply redirects
    |
run builtin in parent
    |
restore original stdin/stdout using dup2()
    |
close saved descriptors
```

If stdout is not restored, the Minishell prompt itself may continue writing into the redirected file.

---

## 14. Signal Behaviour Still Required

Three signal modes are already separated by API:

```text
signals_setup_prompt()
signals_setup_child()
signals_setup_heredoc()
```

Keep them separate.

### Interactive prompt

Expected general behaviour:

* `Ctrl-C`: cancel current input, show a new prompt, set appropriate status.
* `Ctrl-\`: ignored at the prompt.
* `Ctrl-D`: readline returns `NULL`; exit cleanly.

### Child processes

Children should normally restore/default relevant signal behaviour so programs such as `cat` react normally.

### Heredoc

`Ctrl-C` should interrupt heredoc collection and return control to the shell without killing Minishell itself.

Only the allowed signal-related global should be used for signal notification. Do not put general shell state into globals.

---

## 15. Main Loop Still Needs Integration

The current `main.c` initializes the shell and reads lines, but it does not yet run the implemented frontend.

The final loop should conceptually become:

```text
readline
  |
add_history
  |
lex
  |
parse
  |
free tokens
  |
expand
  |
heredoc_collect
  |
execute
  |
free commands
  |
next prompt
```

Every failure path must free whichever structures have already been allocated.

Do not make `main.c` responsible for the internals of lexing, parsing, expansion, or execution. It should coordinate stages.

---

## 16. Memory Ownership Cheat Sheet

| Object                       | Created by         | Owned/freed by                      |
| ---------------------------- | ------------------ | ----------------------------------- |
| `t_token` node/value         | lexer              | `free_tokens()`                     |
| `t_command`                  | parser             | `free_commands()`                   |
| `argv[i]` before expansion   | parser             | command; replaced/freed by expander |
| `argv[i]` after expansion    | expander           | `free_commands()`                   |
| `t_redir` / target           | parser             | `free_commands()`                   |
| expanded redirect target     | expander           | `free_commands()`                   |
| `t_env` nodes                | environment layer  | `free_env()`                        |
| pointer from `env_get()`     | environment        | **borrowed; never free directly**   |
| array from `env_to_array()`  | environment helper | caller                              |
| string from `path_resolve()` | executor           | caller/child failure path           |
| heredoc fd                   | heredoc/runtime    | executor/cleanup path               |

When debugging a leak or double free, start with this table rather than randomly adding `free()` calls until Valgrind becomes a different colour.

---

## 17. Existing Tests

The Makefile currently exposes:

```bash
make test_env
make test_lexer
make test_builtin
make test_parser
make test_expander
make test_exec
```

or:

```bash
make tests
```

### Confirmed during development

Parser validation previously completed with:

```text
251 allocations
251 frees
0 bytes in use at exit
0 Valgrind errors
```

Word-expander testing completed with:

```text
64 allocations
64 frees
0 bytes in use at exit
0 Valgrind errors
```

The current builtin test completed with strict Valgrind using:

```text
15 allocations
15 frees
0 bytes in use at exit
0 Valgrind errors
exit code 0
```

The frontend integration test has also demonstrated:

```bash
echo "$USER" '$USER' $?
echo abc"$USER" "$HOME/test" | cat
cat < "$HOME/input" > '$USER.out'
cat << "EOF"
```

with the expected expanded command/redirection structures.

---

## 18. Recommended Validation Command Set

Before every handover/merge:

```bash
norminette src include
make fclean
make
make tests
```

For a targeted memory test:

```bash
valgrind \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    --errors-for-leak-kinds=all \
    --error-exitcode=42 \
    ./test_name

echo $?
```

Expected strict result:

```text
All heap blocks were freed
ERROR SUMMARY: 0 errors
exit code: 0
```

Once the real shell is integrated, test the executable itself as well. Readline can produce known library-related reports depending on environment, so distinguish project-owned leaks from external/library allocations rather than blindly suppressing everything.

---

## 19. Final Behaviour Checklist

Russell should compare Minishell directly against Bash for at least these groups.

### Basic execution

```bash
/bin/echo hello
ls
ls -la
nonexistent_command
```

### Quotes and expansion

```bash
echo '$USER'
echo "$USER"
echo "$?"
echo abc"$USER"xyz
echo "$DOES_NOT_EXIST"
```

### Pipes

```bash
echo hello | cat
printf hello | wc -c
cat /etc/passwd | grep root | wc -l
```

### Redirections

```bash
echo hello > out
cat < out
echo again >> out
cat < out > copy
```

### Multiple redirections

```bash
echo hello > a > b
cat < a < b
```

### Heredoc

```bash
cat << EOF
cat << "EOF"
cat << '$USER'
```

### Builtins

```bash
echo hello
echo -n hello
pwd
cd /tmp
export TEST=hello
env
unset TEST
exit
```

### Exit/status

```bash
false
echo $?
true
echo $?
nonexistent_command
echo $?
```

### Signals

Test `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` at:

* empty prompt;
* while `cat` is running;
* inside a heredoc;
* during pipelines.

---

## 20. Suggested Remaining Work Breakdown

### Priority 1 — Complete runtime foundation

```text
path_resolve
status_decode/status_collect
redirect_apply
```

### Priority 2 — Processes

```text
child_run
pipeline_run
execute
```

### Priority 3 — Stateful builtins

```text
cd
export
exit
```

These should be designed alongside parent-vs-child builtin execution rather than independently.

### Priority 4 — Heredoc + signals

```text
heredoc_collect
signals_setup_prompt
signals_setup_child
signals_setup_heredoc
```

### Priority 5 — Main integration

Wire:

```text
lexer -> parser -> expander -> heredoc -> executor
```

into the readline loop.

### Priority 6 — Hardening

* Bash comparison;
* invalid syntax;
* file permission failures;
* missing PATH;
* command-not-found handling;
* pipeline exit statuses;
* allocation failures where practical;
* descriptor leaks;
* Norminette;
* full Valgrind pass.

---

## 21. Design Principles to Preserve

### Keep modules narrow

The lexer identifies tokens. It does not execute commands.

The parser builds command structures. It does not expand variables.

The expander transforms words. It does not fork.

The executor coordinates execution. It should delegate path, pipes, redirects and statuses to their respective helpers.

This separation makes failures easier to isolate and helps stay inside the 42 function limits.

### Avoid hidden ownership changes

If a function replaces a pointer, make ownership clear.

If a function returns borrowed memory, do not free it.

If a function returns newly allocated memory, document who frees it.

### Fail upward

Helpers should generally report failure and let their caller decide how far the shell must unwind.

Do not terminate the entire Minishell process from an arbitrary helper just because an allocation or `open()` failed.

### Preserve shell state intentionally

Ask this question whenever implementing a builtin:

> Does this operation need to change the parent shell after the command returns?

If yes, execution placement matters.

---

## 22. Evaluation Understanding — Questions We Should Be Able to Answer

Finishing the code is only half the job. Both partners should be able to explain these without memorised one-line definitions:

1. Why does the lexer keep quotation characters instead of stripping them immediately?
2. Why does the parser duplicate token strings?
3. Who owns `argv[i]` before and after expansion?
4. Why does `env_get()` return borrowed memory while `expand_variable()` duplicates it?
5. Why does single-quoted `$USER` behave differently from double-quoted `$USER`?
6. Why is the heredoc delimiter not variable-expanded?
7. What exactly does `expand_body` record, and when is it used?
8. Why must `cd`, `export`, `unset`, and `exit` sometimes execute in the parent?
9. Why can those same builtins execute in children when part of a pipeline?
10. Why must unused pipe descriptors be closed in both parent and child?
11. Why must the shell use the status of the last pipeline command?
12. What is the ownership of the array returned by `env_to_array()`?
13. Why does successful `execve()` never return?
14. What is the difference between command-not-found and permission-denied execution failures?
15. Why should heredocs generally be collected before pipeline execution?
16. Why are prompt, child, and heredoc signal configurations separate?
17. What happens if a redirection fails before command execution?
18. Why does redirection order matter?
19. What cleanup is required when parsing succeeds but expansion fails?
20. Why is `t_shell` persistent while `t_token` and `t_command` are per-input structures?

If both partners can explain the *reason* behind these answers, not merely state what the function does, the project is in a much stronger position for evaluation.

---

## 23. Handover Summary for Russell

The safest assumption at handover is:

```text
FRONTEND
Environment       DONE
Lexer             DONE
Parser            DONE
Expander          DONE
Cleanup           DONE

CORE BUILTINS
Dispatch          DONE
echo              DONE
pwd               DONE
env               DONE
unset             DONE

RUSSELL'S MAIN COMPLETION AREA
cd                TODO
export            TODO
exit              TODO
PATH resolution   TODO
executor          TODO
pipes             TODO
redirections      TODO
heredoc runtime   TODO
signals           TODO
main integration  TODO
final hardening   TODO
```

Do not rewrite completed frontend modules unless a failing integration test demonstrates a real contract problem.

Build the remaining runtime around the established structures and ownership rules first. Refactor only when there is a concrete reason.

---

## 24. Final Definition of Done

The project is not complete merely because all stub files contain code.

It is complete when:

* required Minishell behaviour works interactively;
* lexer/parser/expander feed execution correctly;
* all seven builtins match required behaviour;
* external commands resolve and execute;
* redirections work in correct order;
* pipelines work without descriptor hangs/leaks;
* heredocs respect quoted/unquoted delimiter rules;
* exit statuses and `$?` are correct;
* signal behaviour is appropriate in prompt/child/heredoc modes;
* parent-state builtins persist correctly;
* memory and file descriptors are clean;
* Norminette passes;
* `make`, `clean`, `fclean`, and `re` behave correctly;
* both partners can explain the design decisions and ownership model.

At that point, the shell is not merely running. It is defensible.
