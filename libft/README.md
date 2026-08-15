_This project has been created as part of the 42 curriculum by rfoo._

# Libft

## Description

`libft` is a custom C library built from scratch as part of the 42 curriculum. It reimplements essential functions from the standard C library including memory manipulation, string handling, character checks, and linked list operations without relying on external dependencies.

This project serves as a deep dive into low-level programming fundamentals and will be used as the foundation for other 42 projects in the future.

This version of `libft` includes `ft_printf` and `get_next_line` modules after completing them as part of the 42 curriculum. 

## Features

### Libc functions

| Function | Description |
|----------|-------------|
| `int ft_isalpha(int c);` | Checks if a character is alphabetic (`A-Z` or `a-z`). Returns nonzero if true. |
| `int ft_isdigit(int c);` | Checks if a character is a decimal digit (`0-9`). |
| `int ft_isalnum(int c);` | Checks if a character is alphanumeric (letter or digit). |
| `int ft_isascii(int c);` | Checks if a character is within the ASCII range (0–127). |
| `int ft_isprint(int c);` | Checks if a character is printable (including space). |
| `size_t ft_strlen(const char *s);` | Returns the length of a string (excluding the null terminator). |
| `void *ft_memset(void *s, int c, size_t n);` | Fills a block of memory with a byte value. |
| `void ft_bzero(void *s, size_t n);` | Sets a block of memory to zero. |
| `void *ft_memcpy(void *dest, const void *src, size_t n);` | Copies `n` bytes from source to destination (no overlap handling). |
| `void *ft_memmove(void *dest, const void *src, size_t n);` | Copies `n` bytes safely, handling overlapping memory regions. |
| `size_t ft_strlcpy(char *dst, const char *src, size_t size);` | Copies a string into a buffer with size limit, always null‑terminating if size > 0. |
| `size_t ft_strlcat(char *dst, const char *src, size_t size);` | Appends a string to a buffer with size limit, ensuring null termination. |
| `int ft_toupper(int c);` | Converts a lowercase letter to uppercase. |
| `int ft_tolower(int c);` | Converts an uppercase letter to lowercase. |
| `char *ft_strchr(const char *s, int c);` | Finds the first occurrence of a character in a string. |
| `char *ft_strrchr(const char *s, int c);` | Finds the last occurrence of a character in a string. |
| `int ft_strncmp(const char *s1, const char *s2, size_t n);` | Compares two strings up to `n` characters. |
| `void *ft_memchr(const void *s, int c, size_t n);` | Searches for a byte value in memory. |
| `int ft_memcmp(const void *s1, const void *s2, size_t n);` | Compares two memory blocks. |
| `char *ft_strnstr(const char *big, const char *little, size_t len);` | Locates a substring within a string, searching up to `len` characters. |
| `int ft_atoi(const char *nptr);` | Converts a string to an integer. |
| `void *ft_calloc(size_t nmemb, size_t size);` | Allocates memory for an array and initialises it to zero. |
| `char *ft_strdup(const char *s);` | Duplicates a string by allocating new memory and copying its contents. |

### Additional functions

| Function | Description |
|----------|-------------|
| `char *ft_substr(char const *s, unsigned int start, size_t len);` | Allocates and returns a substring from string `s`, starting at index `start` and of maximum length `len`. |
| `char *ft_strjoin(char const *s1, char const *s2);` | Allocates and returns a new string that is the concatenation of `s1` and `s2`. |
| `char *ft_strtrim(char const *s1, char const *set);` | Allocates and returns a copy of `s1` with characters from `set` removed from the beginning and end. |
| `char **ft_split(char const *s, char c);` | Splits string `s` into an array of strings using character `c` as the delimiter. |
| `char *ft_itoa(int n);` | Converts an integer `n` into a string representation. |
| `char *ft_strmapi(char const *s, char (*f)(unsigned int, char));` | Applies function `f` to each character of string `s`, creating a new string with the results. |
| `void ft_striteri(char *s, void (*f)(unsigned int, char*));` | Applies function `f` to each character of string `s`, modifying the string in place. |
| `void ft_putchar_fd(char c, int fd);` | Writes character `c` to the given file descriptor `fd`. |
| `void ft_putstr_fd(char *s, int fd);` | Writes string `s` to the given file descriptor `fd`. |
| `void ft_putendl_fd(char *s, int fd);` | Writes string `s` followed by a newline to the given file descriptor `fd`. |
| `void ft_putnbr_fd(int n, int fd);` | Writes integer `n` as a string to the given file descriptor `fd`. |
| `void ft_swap(int *a, int *b);` | Swaps the values of two integers pointed to by `a` and `b`. |
| `long ft_atol(const char *str);` | Converts the string `str` to a `long` integer value. Similar to `atoi`, but returns a `long`. |
| `int ft_isdouble(const char *str);` | Checks if the string `str` represents a valid double (floating‑point number). Returns nonzero if true, zero otherwise. |
| `double ft_atof(const char *str);` | Converts the string `str` to a `double` floating‑point value. |

### Linked list functions

| Function | Description |
|----------|-------------|
| `t_list *ft_lstnew(void *content);` | Allocates and returns a new list node, with its `content` initialized to the given value and `next` set to `NULL`. |
| `void ft_lstadd_front(t_list **lst, t_list *new);` | Adds the node `new` at the beginning of the list `lst`. |
| `int ft_lstsize(t_list *lst);` | Counts and returns the number of nodes in the list. |
| `t_list *ft_lstlast(t_list *lst);` | Returns the last node of the list. |
| `void ft_lstadd_back(t_list **lst, t_list *new);` | Adds the node `new` at the end of the list `lst`. |
| `void ft_lstdelone(t_list *lst, void (*del)(void *));` | Frees the memory of a single node’s `content` using `del`, then frees the node itself. |
| `void ft_lstclear(t_list **lst, void (*del)(void *));` | Deletes and frees all nodes of the list using `del`, leaving `lst` as `NULL`. |
| `void ft_lstiter(t_list *lst, void (*f)(void *));` | Iterates over the list and applies function `f` to the `content` of each node. |
| `t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));` | Creates a new list by applying function `f` to each node’s `content`. Uses `del` to free memory if allocation fails. |

### ft_printf

| Function | Description |
|----------|-------------|
| `int ft_printf(const char *s, ...);` | Custom implementation of `printf`. Parses the format string `s` and prints arguments accordingly. |

### get_next_line

| Function | Description |
|----------|-------------|
| `char *get_next_line(int fd);` | Reads from the file descriptor `fd` and returns the next line, including the terminating newline character if present. Returns `NULL` when there are no more lines to read or on error. |

## Instructions

### Build the library

Run:

```
make
```

This compiles all source files into object files and archives them into a static library, `libft.a`.

### Using the library

Run the following command with your `main.c`:
```
cc -Wall -Wextra -Werror -o my_program main.c libft.a
```

Then, to run the program:
```
./my_program
```

## Resources
- [Makefile tutorial](https://makefiletutorial.com/)
- [Linus man pages](https://linux.die.net/man/)
- [How to Structure C Projects](https://www.lucavall.in/blog/how-to-structure-c-projects-my-experience-best-practices)