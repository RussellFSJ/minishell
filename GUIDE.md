# 42 Minishell — Handover & Completion Guide

> **Review date:** 18 October 2026\
> **Repository:** [RussellFSJ/minishell](https://github.com/RussellFSJ/minishell) (`master`)\
> **Reviewed commit:** `f30f10839721b58ca9ca6afffa2426bde10eacde`\
> **Audience:** All project contributors and reviewers\
> **Target:** C implementation; Linux development and evaluation\
> **Status:** **OPEN FAILURES — NOT CLEARED FOR SUBMISSION**

This is the shared **handover and completion guide**. It records **what is failing, what is expected, where to make changes, and what counts as finished**. The root [README.md](README.md) remains the short project introduction.

> **Evidence scope:** Findings below combine C source review with saved Linux runtime results from Ubuntu 24.04 on ARM64, GCC 13.3, Bash 5.2, and GNU Readline 8.2. All 139 saved source/header/Makefile fingerprints matched at the latest review. These results describe the reviewed source; they do not certify later changes or every evaluation platform.

The guide is independent of any contributor's workstation. It contains no machine setup, test-reproduction walkthroughs, or commands for operating the review environment. Each contributor can use the team's chosen Linux setup and record evidence against the relevant commit.

---

## 0. START HERE — current position

**Current verdict: NOT CLEARED FOR SUBMISSION.** The main execution flow and all seven mandatory builtins have implementations. Remaining work concerns behavior, resource ownership, error handling, build dependencies, and team tooling.

### Reading this guide

- **§1:** Failed checkpoints — expected behavior, current result, required work.
- **§2–3:** Execution lifecycle, source map, and ownership boundaries.
- **§4–5:** Ordered development work and implementation notes.
- **§6–8:** Edge-case review, completion criteria, and group handover.
- **§9:** Existing evidence supporting the findings.

### Tracking rules

**All F01–F14 checkpoints are OPEN and unassigned.** Assign an owner when work starts. Use `OPEN → IN PROGRESS → VERIFYING → CLOSED` in the team's issue tracker or pull request.

Only failed checkpoints remain in §1. Remove a resolved row after review and acceptance; keep its ID, fix commit, reviewer, and evidence in the issue or PR. Never reuse an ID. Passing checks and unverified suspicions do not belong in the failure table.

**Priority:** P1 = fix before functional sign-off. P2 = compatibility, explicit cleanup, or development-workflow work. These are project priorities, not official 42 grading categories. No completion percentage is inferred from the number of rows.

---

## 1. Failed checkpoints — expected vs current

### Expansion and redirection

| Checkpoint | What is expected | Current result | What needs to be done |
|---|---|---|---|
| **F01 · P1** Argument expansion | Unquoted variables can produce zero, one, or multiple arguments. Quoted empty arguments remain. | A space-containing variable stays one argument. Unquoted empty values insert empty arguments; an empty command prefix causes status 127. | Preserve quote context and rebuild argv with the correct field count. Update `expand_argv.c`, `expand_word.c`, declarations, and callers. |
| **F02 · P1** Ambiguous redirects | An unquoted redirect operand must produce exactly one field; otherwise report ambiguity with status 1. | A multi-field value becomes a literal filename; an unset value reaches `open("")`. | Validate expanded fields in `expand_redirs.c`; distinguish ambiguity from allocation failure and preserve redirection order. |

### Heredocs and resource ownership

| Checkpoint | What is expected | Current result | What needs to be done |
|---|---|---|---|
| **F03 · P1** Heredoc interruption | Ctrl-C cancels collection with status 130 and restores the terminal. | Status is 130, but terminal flags retain `-icanon -echo` and changed input settings. | Restore saved terminal state on all parent return paths in `heredoc_run.c`; coordinate signal handling. |
| **F04 · P1** Inherited heredoc descriptors | Children inherit only intended descriptors after redirection setup. | The controlled external child has FDs `[0,1,2,4]` after a heredoc instead of `[0,1,2]`. | Close consumed and unrelated heredoc FDs before child execution; update redirect/executor ownership contracts. |
| **F12 · P2** Explicit child cleanup | Ordinary child exits release project-owned allocations under the project's cleanup policy. | Heredoc and builtin children exit with command, expansion, and environment allocations still reachable. | Provide full-context child cleanup in `executor_child_run.c` and `heredoc_run.c`; preserve status and avoid double cleanup. |

**F12 qualification:** These are reachable allocations, not proven lost memory or memory corruption. Explicit release before ordinary child exit is a project quality criterion; confirm the assigned evaluation policy separately. Readline-owned allocations require separate interpretation.

### Builtins

| Checkpoint | What is expected | Current result | What needs to be done |
|---|---|---|---|
| **F05 · P1** Deleted working directory | Navigation can leave an unlinked current directory when the destination is reachable. | `cd` aborts when the initial `getcwd()` fails, before trying `chdir()`. | Separate old-path lookup from navigation; define PWD/OLDPWD fallback and update failures in `builtin_cd.c`. |
| **F06 · P2** Exit whitespace | A numeric exit argument with permitted surrounding whitespace is accepted. | `exit ' 42 '` returns 2 instead of 42. | Handle surrounding whitespace while retaining digit validation, overflow checks, and modulo-256 conversion. |
| **F07 · P2** Export escaping | Export listings quote special characters correctly. | Value `a"b` is printed as `declare -x REVIEW_Q="a"b"`. | Escape values in `builtin_export_print.c` without changing stored environment data. |
| **F08 · P2** Logical directory behavior | Default Bash-style navigation follows logical symlink paths; `pwd` and PWD agree. | A symlink is resolved physically; moving to its parent reaches the physical rather than logical parent. | Coordinate logical-path tracking in `builtin_cd.c` and `builtin_pwd.c`; handle stale or user-edited PWD. |
| **F09 · P1** Output failures | Builtins diagnose failed output and return nonzero. | `echo`, `pwd`, `export`, and `env` silently return 0 when `/dev/full` rejects writes. | Add checked C output helpers and propagate failures through builtin return values to shell status. |

**F08 qualification:** This is a demonstrated Bash compatibility difference. Agree its required scope against the assigned subject before choosing the implementation. For F09, the requirement is nonzero status and a useful diagnostic; GNU `env`'s particular failure code is not imposed on the builtin.

### Build and team workflow

| Checkpoint | What is expected | Current result | What needs to be done |
|---|---|---|---|
| **F10 · P1** Incremental builds | Header changes rebuild affected objects; libft changes rebuild the archive and relink minishell. | Root make reports no work for simulated changes to `include/types.h` and `libft/src/ft_strlen.c`. | Add header dependencies and reliable delegation to libft's Makefile; retain an idle unchanged build. |
| **F11 · P2** Strict instrumented build | The optimized GCC sanitizer build compiles with warnings treated as errors. | Ignored `write()` results fail the `-O1 -Werror` build; recorded sanitizer execution needed a warning downgrade. | Handle write results appropriately in libft output, EOF output, heredoc interruption, and the prompt handler. |
| **F13 · P2** Regression gate | The shared test entry point reports failure for regressions, harness errors, and failed required checks. | No `make test` target; review collectors can exit 0 despite differences or failed stages. | Agree a portable team test entry point and add explicit assertions/failure propagation. Keep test dependencies separate from the C build. |
| **F14 · P2** README accuracy | Documentation links and status reflect the current repository. | README links to absent `test.md` and describes verification as not performed. | Link this guide, update dated status, and document the team's chosen workflow once implemented. |

**F11 qualification:** The ordinary production build was not blocked by this warning issue. F13/F14 are development-workflow gaps, not missing shell language features.

---

## 2. Execution lifecycle — how the pieces fit

```text
main() → minishell(envp)
             │
             ├─ readline() and history
             ├─ lex() → tokens
             ├─ parser_parse() → command list
             ├─ free_tokens()
             ├─ expand() → argv and redirection targets
             ├─ executor_run()
             │      ├─ collect heredocs
             │      ├─ standalone builtin → parent + stdio restoration
             │      └─ pipeline → fork / redirect / builtin or execve
             │                      → wait, status, descriptor cleanup
             ├─ free_commands()
             └─ next prompt; environment survives until shutdown
```

### Boundaries to preserve

- **Parent state:** Standalone state-changing builtins must affect the shell. Pipeline builtins run in children and must not change the parent's environment or directory.
- **Parser ownership:** Parsed command strings are owned independently of tokens. Freeing tokens must not invalidate argv or redirection targets.
- **Expansion ownership:** Replacing strings or vectors must preserve a valid cleanup path, including partial allocation failures.
- **Pipeline lifetime:** Parent and children close unused pipe ends; every child must be reaped. Pipeline status comes from the last command.
- **Heredoc lifetime:** Collection precedes execution. All inherited heredoc descriptors need explicit ownership across parent and child paths.

These describe implementation boundaries, not additional passing checkpoints.

---

## 3. Source map and C ownership contracts

### Where to work

| Area | Files / directories | Related failures |
|---|---|---|
| Expansion | `src/expander/`, `include/expander.h` | F01, F02 |
| Heredocs / redirection | `src/redirect/` | F02, F03, F04, F12 |
| Signals | `src/signals/` | F03, F11 |
| Builtins | `src/builtins/`, `include/builtins.h` | F05–F09 |
| Execution / child lifetime | `src/executor/`, `include/executor.h` | F04, F12 |
| Cleanup / environment | `src/cleanup/`, `src/env/` | F01, F05, F12 |
| Shared types / loop | `include/types.h`, `src/minishell.c` | Shared interfaces, F11 |
| Build / output helpers | `Makefile`, `libft/Makefile`, `libft/src/` | F09–F11 |
| Team tooling / documentation | `review/`, `README.md`, `GUIDE.md` | F13, F14 |

### Ownership rules

| Data / API | Contract to preserve |
|---|---|
| `t_shell.env` | Owned across prompts; released at shell shutdown or appropriate child finalization. |
| `env_get()` | Returns a borrowed value. Callers must not free it or retain it across invalidating environment changes. |
| `env_to_array()` | Returns a newly allocated environment array; release it after failed execution. |
| `t_command.argv` | Owned, NULL-terminated when present. Redirection-only commands may have no argv. |
| `t_redir` | Owns its target string. A valid heredoc FD needs explicit closure; `fd == -1` represents no active descriptor. |
| `free_redirs()` | Currently frees memory, not FDs. Adding closure requires coordination with existing executor cleanup. |
| `executor_path_resolve()` | Returns an owned path; execution failure must release temporary path/environment allocations. |
| `parser_parse()` | Uses `-1` for allocation failure and `-2` for syntax errors. Preserve the current distinction. |

**Shared-interface rule:** Agree changes to structs, headers, argument ownership, and return codes with the affected module owners. Update all callers in the same coherent change.

---

## 4. Ordered completion work

### Phase A — make development reliable

**F10 first:** ensure changed headers and libft sources rebuild correctly. Start F13 alongside implementation work so regressions are visible to every contributor.

**Finished when:** builds use current source, unchanged builds do no unnecessary work, and the agreed regression gate cannot silently succeed after a required failure.

### Phase B — correct expansion

**F01 + F02 together:** define quote-aware expansion results, variable argument counts, redirect field validation, and error propagation.

**Finished when:** argument and redirect semantics match the chosen supported behavior, explicit empty strings are preserved, and allocation failures leave valid ownership.

### Phase C — repair interactive and resource behavior

**F03 + F04, followed by F12:** restore terminal state, close inherited descriptors, and provide complete ordinary-child cleanup.

**Finished when:** interrupted heredocs leave a usable terminal, external children inherit no unintended shell descriptors, and cleanup follows the agreed memory policy.

### Phase D — finish builtin edge cases

**F05 + F09 first; then F06–F08 and F11:** correct directory recovery, write-error reporting, exit parsing, export representation, and agreed logical-directory behavior.

**Finished when:** errors produce accurate status/diagnostics, later commands remain usable, and stricter compilation no longer needs the ignored-result downgrade.

### Phase E — integrate and hand over

**F13 + F14:** complete the shared acceptance workflow and README updates. Have a contributor other than the author review shared-interface and ownership changes.

**Finished when:** evidence identifies the integrated revision, documentation reflects it, and all applicable open checkpoints are closed or explicitly scoped out with a recorded rationale.

---

## 5. Implementation notes — avoid incomplete fixes

### F01/F02 — preserve context until field formation

`expand_argv(t_shell *, char **)` currently replaces one string with one string. It cannot adjust argument count. Introduce an owned expansion result and a way to replace the argv vector, such as a command pointer or `char ***`.

Preserve quote provenance until splitting is complete. A variable's contents are data, not a second round of shell syntax. Build replacements before discarding the old vector, and release partial results on failure.

Treat export assignment operands and heredoc delimiters/body expansion according to their own rules. Preserve left-to-right redirection effects; validating everything in advance can change which earlier redirections take effect.

### F03/F04/F12 — coordinate terminal, FD, and memory ownership

The heredoc child can terminate before Readline restores terminal settings. The parent needs an appropriate restoration path for normal completion, interruption, and setup/wait failure, using APIs allowed by the subject.

Each child inherits descriptors for all previously collected heredocs. Closing only its own active redirection does not close unrelated inherited descriptors. Handle descriptor reuse and descriptors already equal to stdin/stdout; invalidate stored descriptors after closure.

Child finalization needs the complete owned context, not just the current command node. Retain the intended exit status. Successful `execve()` replaces process memory; ordinary exits need the explicit cleanup policy. Signal handlers must not invoke arbitrary allocation or cleanup routines.

### F05/F08 — keep directory state coherent

Failure to discover the old physical path must not automatically prevent navigation. Define logical cwd, physical cwd, PWD, and OLDPWD behavior together. Handle unset or user-edited PWD without blindly trusting it.

If `chdir()` succeeds but an environment update fails, the process has already moved. Define and report that partial failure accurately; avoid stale-`errno` diagnostics.

### F06/F07 — preserve existing builtin contracts

Exit parsing must require digits, reject internal whitespace/junk, retain overflow-safe accumulation, and apply modulo conversion. Accepting surrounding whitespace must not weaken numeric validation.

Export formatting must escape special characters without changing stored values. Preserve sorting and the distinction between an exported name with no value and an explicitly empty value.

### F09/F11 — propagate output errors in C

Use checked output helpers with appropriate `ssize_t` handling, short-write support, and `EINTR` handling. Propagate failure through builtin dispatch to shell status. Shared void libft print interfaces cannot report failure without an interface change; a shell-specific helper can avoid changing unrelated callers.

Handle signal-handler output separately where required for safety. Suppressing compiler warnings or ignoring the result through a cast does not resolve the underlying failure.

### F10/F13/F14 — make results trustworthy

Track header dependencies and invoke libft's dependency logic even when its archive already exists. Avoid forcing an unnecessary final link on every build.

The shared regression gate must distinguish semantic failures from harmless diagnostic wording differences. It must also distinguish a skipped required check from success. Documentation should describe the workflow the team actually implements.

---

## 6. Missed edge cases — review scope

The following are **investigation topics, not confirmed failures**. Add a new checkpoint only after establishing the defect and its expected behavior.

- **Expansion:** quoted/unquoted empty values, whitespace-only values, adjacent fragments, quote characters supplied by variables, expanded command names, and export assignment context.
- **Redirections/heredocs:** multiple operands, later redirects replacing earlier ones, quoted delimiters, interrupted later heredocs, EOF, and descriptors shared across pipeline stages.
- **Builtins:** deleted directories, unset or edited HOME/PWD/OLDPWD, partial environment-update failures, numeric boundaries, special export values, and output failures in parent versus pipeline contexts.
- **C allocation paths:** partial argv/environment/list construction, NULL termination, size overflow, borrowed pointers, and preservation of `errno` before cleanup.
- **Startup:** distinguish an empty environment from allocation failure; `env_init()` can currently return NULL for either condition.
- **Processes and FDs:** failed allocation/open/pipe/duplication/fork/wait operations, closed standard descriptors, descriptor reuse, child reaping, and repeated failures without resource growth.
- **Integration:** parent-state persistence, command lookup, last-stage pipeline status, signal timing, and usability after an error.

Use the assigned subject to define scope. Full Bash syntax is not automatically required. Current cosmetic diagnostic differences and older resolved parser findings are not reopened here.

---

## 7. Definition of done

A checkpoint is complete when its expected behavior is established on the candidate revision, related behavior remains intact, ownership/error paths are reviewed, and another contributor can understand the change from its recorded evidence.

**Project sign-off requires:**

- All applicable failed checkpoints resolved; any scope exclusion explicitly agreed and documented.
- A clean Linux build, correct incremental dependencies, and required compiler warnings enforced.
- Norminette and allowed-function compliance against the assigned subject.
- Functional, interactive-terminal, memory, descriptor, and process-lifecycle evidence for the final integrated source.
- Accurate documentation and a shared acceptance workflow that reports failures reliably.
- Contributors able to explain module interfaces, parent/child behavior, resource ownership, and failure cleanup.

**Current sign-off: OPEN.** This is a completion standard, not a list of newly observed failures. Results from an earlier commit remain historical once relevant source changes.

---

## 8. Group collaboration and handover

### Working agreement

1. Assign one owner per checkpoint and identify affected modules before concurrent work.
2. Agree shared-header, ownership, and return-contract changes with every affected contributor.
3. Keep fixes scoped; follow the assigned C/Norminette rules and permitted-function list.
4. Record results against the candidate commit and Linux environment used by that contributor.
5. Have a reviewer assess behavior, cleanup, and related regressions before closure.
6. After integration, update the active failure table and preserve the closure record in the issue or PR.

### Handover record

| Field | Information to record |
|---|---|
| Checkpoint | Stable F-ID and short description |
| Ownership | Author and reviewer |
| Revision | Fix commit and integrated commit, if different |
| Change | Affected files, behavior, and any interface changes |
| Evidence | Observed outcome, Linux environment, and shared report/PR link |
| Remaining work | Unresolved cases, skipped verification, or agreed scope decisions |
| State | OPEN, IN PROGRESS, VERIFYING, or CLOSED |

### Questions every contributor should be able to answer

- Why do standalone state-changing builtins run in the parent?
- Who owns tokens, argv, environment values, and redirection descriptors?
- Which pipe/heredoc descriptors must each process close?
- How are syntax, allocation, execution, redirection, and signal statuses distinguished?
- How do quoted and unquoted expansion differ?
- What happens when a resource operation fails partway through a command?

---
