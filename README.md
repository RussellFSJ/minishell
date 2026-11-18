*This project has been created as part of the 42 curriculum by ebin-ahm, rfoo.*

# Minishell

A small Bash-inspired command interpreter written in C for the 42 curriculum.

## Description

Minishell reads commands, interprets quotes and operators, expands environment
variables, and executes builtins or external programs. The project develops an
understanding of processes, signals, memory ownership, and file descriptors.

Supported features include

- An interactive prompt and command history.
- Executable lookup through PATH, relative paths, and absolute paths.
- Single quotes that preserve literal text.
- Double quotes that preserve spaces while allowing variable expansion.
- Environment-variable expansion and `$?` for the last foreground pipeline's
  exit status.
- Field splitting of unquoted variable expansions, with quoted empty arguments
  preserved.
- Input redirection with `<`, output redirection with `>`, and append
  redirection with `>>`.
- Heredocs with `<<`, including quoted delimiters that preserve the body
  literally. Heredoc body lines are excluded from command history.
- Pipelines connecting multiple commands.
- The builtins `echo` with `-n`, `cd`, `pwd`, `export`, `unset`, `env`, and
  `exit`.

Standalone builtins run in the parent process so changes to the working
directory and environment persist. Commands in pipelines run in child
processes.

| Input | At the prompt | While running `cat` |
| --- | --- | --- |
| Ctrl-C | Clear input and display a new prompt | Interrupt `cat` and set `$?` to 130 |
| Ctrl-D | Exit when the input is empty | Send EOF when the current input line is empty |
| Ctrl-\ | Ignore | Terminate `cat` through SIGQUIT and set `$?` to 131 |

During heredoc input, Ctrl-C cancels collection and sets `$?` to 130.
Ctrl-\ is ignored. External-command children start with the default SIGINT
and SIGQUIT handlers, which the executed program can change.

The command lifecycle is

```text
readline → lexer → parser → expansion → execution → cleanup
```

## Instructions

### Build and run

```sh
make
./minishell
```

The Makefile uses `cc` with `-Wall -Wextra -Werror` and builds the bundled
Libft before linking Minishell.

| Command | Action |
| --- | --- |
| `make` | Build Minishell |
| `make clean` | Remove object files |
| `make fclean` | Remove object files, the library archive, and executable |
| `make re` | Rebuild from scratch |

### Builtins

| Command | Usage |
| --- | --- |
| `echo` | Print arguments, with `-n` to omit the trailing newline |
| `cd` | Change directory using a relative or absolute path, or HOME when omitted |
| `pwd` | Print the current working directory |
| `export` | Set exported variables, or list them when arguments are omitted |
| `unset` | Remove environment variables |
| `env` | Print exported variables that have values |
| `exit` | Exit with the previous status or a supplied numeric status |

### Usage

Enter commands at the `minishell$` prompt.

```sh
echo hello
export MESSAGE="hello world"
echo "$MESSAGE"
echo "$MESSAGE" | wc -w
false
echo $?
cd /tmp
pwd
exit
```

To enter a heredoc, type the following lines separately.

```sh
cat << END
hello from the heredoc
END
```

An unquoted heredoc delimiter enables variable expansion in its body.
Quoting the delimiter preserves the body literally.

## Project layout

```text
include/                   Shared types and module declarations
libft/                     Supporting C library
src/
├── main.c                 Program entry point
├── minishell.c            Interactive loop and command lifecycle
├── builtins/              Builtin commands
├── cleanup/               Allocation cleanup and shell shutdown
├── environment/           Environment storage and updates
├── executor/              Command lookup, processes, pipelines, and status
├── expander/              Variables, quoting, and field splitting
├── lexer/                 Tokenization
├── parser/                Command and redirection construction
├── redirections/          File redirections and heredocs
├── shell_io/              Checked output helpers
├── signals/               Prompt, child, and heredoc signal handling
└── working_directory/     Working-directory state and path normalization
```

## Resources

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/) —
  reference behavior for quoting, expansion, redirections, and builtins.
- [GNU Readline](https://tiswww.case.edu/php/chet/readline/rltop.html) —
  interactive input and history.
- [Linux man-pages](https://man7.org/linux/man-pages/) — process, signal,
  filesystem, and descriptor APIs.
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9799919799/utilities/V3_chap02.html) —
  shell terminology and background.

### AI usage

ChatGPT and Codex assisted with explanations of C and Unix concepts, code
review, implementation fixes, refactoring, and documentation.

The authors are responsible for reviewing, understanding, and validating
the submitted implementation.

## Authors

`ebin-ahm` · `rfoo`

[GitHub repository](https://github.com/RussellFSJ/minishell)
