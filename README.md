*This project has been created as part of the 42 curriculum by demane, rbourdon.*

# minishell

A simple Unix shell inspired by `bash`, developed as part of the 42 common core.

## Description

**minishell** (prompt: `minishield-1.0$`) recreates a minimal interactive shell. The goal is to understand how a shell works under the hood: reading a command line, tokenizing it, building an abstract syntax tree (AST), expanding variables and wildcards, then executing the resulting commands with the correct redirections, pipes, and signal behavior.

### Pipeline overview

1. **Lexer** — splits the input into tokens (words, operators, redirections, parentheses) while tracking quote segments.
2. **Parser** — validates syntax and builds an AST (`CMD`, `PIPE`, redirections, `AND` / `OR`, subshells).
3. **Expander** — expands `$VAR` / `$?`, applies quote rules, handles heredoc expansion, and performs pathname expansion (globbing).
4. **Executor** — walks the AST: forks when needed, sets up pipes and redirections, runs builtins in the parent process when appropriate, and resolves external binaries via `PATH`.

### Features

- Interactive prompt with GNU **readline** (line editing + history)
- Non-interactive mode (commands from stdin / pipes)
- Quotes: single (`'`) and double (`"`)
- Environment variables and `$?`
- Pipes (`|`)
- Redirections: `<`, `>`, `>>`, `<<` (heredoc)
- Logical operators: `&&`, `||`
- Subshells: `( ... )`
- Wildcard expansion (`*`)
- Builtins: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Signal handling (`Ctrl-C`, `Ctrl-D`, `Ctrl-\`) consistent with interactive bash behavior where required

### Project structure

```
minishell/
├── Makefile
├── includes/
│   ├── *.h                 # Public headers
│   └── libft/              # Custom C library (libft + ft_printf + GNL)
├── srcs/
│   ├── main.c
│   ├── process_minishell*.c
│   ├── lexer/
│   ├── parser/
│   ├── expander/
│   ├── executor/
│   ├── builtins/
│   ├── env/
│   ├── signals/
│   └── utils/
└── readline.supp           # Valgrind suppressions for readline
```

## Instructions

### Requirements

- A C compiler (`cc` / `gcc`)
- `make`
- GNU **readline** development libraries (`libreadline-dev` on Debian/Ubuntu)

```bash
sudo apt install build-essential libreadline-dev
```

### Compilation

```bash
make        # build libft then minishell
make clean  # remove object files
make fclean # remove objects + binary + libft archive
make re     # fclean + all
```

The binary is produced at the repository root as `./minishell`.

### Execution

```bash
./minishell
```

Example session:

```text
minishield-1.0$ echo "Hello, world!"
Hello, world!
minishield-1.0$ ls | grep .c
minishield-1.0$ export FOO=42 && echo $FOO
42
minishield-1.0$ cat << EOF
> line
> EOF
line
minishield-1.0$ exit
```

You can also feed commands non-interactively:

```bash
echo 'pwd; echo hi' | ./minishell
```

### Memory checks (optional)

A Valgrind suppression file is provided for known readline leaks:

```bash
valgrind --leak-check=full --show-leak-kinds=all \
  --suppressions=readline.supp ./minishell
```

## Library — libft

This project ships a custom **libft** under `includes/libft/`, compiled as `libft.a` and linked into minishell. It is the shared foundation for string/memory handling, output, and line reading.

### Contents

| Module | Role |
|--------|------|
| **libft (mandatory + extras)** | Character tests, memory (`memset`, `memcpy`, `calloc`, …), strings (`strdup`, `substr`, `strjoin`, `split`, `strtrim`, …), conversions (`atoi` / `atol` / `itoa`), FD output helpers, linked lists (`ft_lst*`), and utilities such as `ft_free`, `ft_free_split`, `ft_strjoin_sep`, `ft_str_is_space` |
| **ft_printf** | Formatted printing (`ft_printf`, `ft_printf_fd`) used for debugging and some diagnostics during development |
| **get_next_line** | Line-oriented reading from a file descriptor (bonus variant) |

### Why it matters here

minishell relies heavily on dynamic strings and lists (tokens, segments, environment variables, rebuilt argv after expansion). Centralizing these helpers in libft keeps the shell modules focused on parsing and execution while reusing battle-tested allocation and string APIs. The library is built automatically by the top-level `Makefile` via `make -C includes/libft`.

## Technical choices

- **AST-based execution** instead of a purely linear token walk, which makes pipes, `&&` / `||`, and subshells easier to reason about.
- **Segment-aware tokens** (`SEG_RAW`, `SEG_SQ`, `SEG_DQ`, `SEG_SEP`) so quoting rules survive from lexing through expansion.
- **Environment as a linked list** of key/value nodes with an export flag, then reconstituted into `envp` when launching external commands.
- **Builtins executed in the parent** when they must affect the shell state (`cd`, `export`, `unset`, `exit`, …), and in a child when they appear in a pipeline.

## Resources

### Documentation & references

- [GNU Bash Reference Manual](https://www.gnu.org/software/bash/manual/)
- [POSIX Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/utilities/V3_chap02.html)
- [readline(3)](https://man7.org/linux/man-pages/man3/readline.3.html) — line editing and history
- [signal(7)](https://man7.org/linux/man-pages/man7/signal.7.html) — process signals
- [pipe(2)](https://man7.org/linux/man-pages/man2/pipe.2.html), [fork(2)](https://man7.org/linux/man-pages/man2/fork.2.html), [execve(2)](https://man7.org/linux/man-pages/man2/execve.2.html), [dup2(2)](https://man7.org/linux/man-pages/man2/dup2.2.html)
- [Writing a Unix Shell](https://brennan.io/2015/01/16/write-a-shell-in-c/) — high-level overview of shell architecture
- 42 subject: *minishell*

### Use of AI

AI assistants (including Cursor) were used as support tools during the project, mainly for:

- Clarifying system call and bash behavior (quotes, expansion order, signals, exit statuses)
- Helping debug edge cases (redirections in pipelines, heredoc quoting, memory leaks)
- Reviewing or refactoring small code sections after Norminette / compilation errors
- Drafting and structuring this `README.md`

AI was **not** used as a drop-in code generator for the whole shell: design of the lexer/parser/AST, builtin logic, and execution flow were implemented and validated by the team (`demane`, `rbourdon`).

---

*42 Paris — Common Core*
