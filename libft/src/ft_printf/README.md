_This project has been created as part of the 42 curriculum by rfoo._

# ft_printf

## Description

In this project, we will develop `ft_printf`, a custom implementation of the standard C library function `printf`. The goal is to reproduce its core functionality by parsing format strings and handling variable arguments, while gaining a deeper understanding of variadic functions, modular design, and low‑level output handling.

## Features

The **`ft_printf`** function supports a subset of the standard C `printf` conversion specifiers. Each specifier corresponds to a particular type of argument and output format:

| Specifier | Description                                               |
| --------- | --------------------------------------------------------- |
| %c        | Prints a single character                                 |
| %s        | Prints a string (as defined by the common C convention)   |
| %p        | Prints a `void *` pointer argument in hexadecimal format  |
| %d        | Prints a decimal (base 10) number                         |
| %i        | Prints an integer in base 10                              |
| %u        | Prints an unsigned decimal (base 10) number               |
| %x        | Prints a number in hexadecimal (base 16) lowercase format |
| %X        | Prints a number in hexadecimal (base 16) uppercase format |
| %%        | Prints a percent sign                                     |

## Dictionary and Handlers

To keep the implementation modular, `ft_printf` uses a dictionary structure that maps each format specifier to a corresponding handler function.

### Dictionary (t_dict)

Stores an array of entries, each containing a specifier key (e.g. 'd', 's') and a function pointer.

Provides `dict_set` to register handlers and `dict_get` to retrieve them during parsing.

Ensures that each specifier is linked to the correct output logic.

#### Array of entries instead of Linked List

Given the small and fixed number of specifiers that this version of `ft_printf` handles, a simple array of dictionary entries is more efficient and easier to manage than a linked list.

This design offers several advantages:

- **Constant-time lookup**: Iterating over a small array of specifiers is fast and predictable.
- **Simplicity**: No need for dynamic memory allocation or pointer manipulation as in a linked list.
- **Readability**: The mapping between specifiers and handlers is clear and compact.
- **Fixed size**: Since the set of supported specifiers is known in advance, an array is sufficient and avoids the overhead of a more complex data structure.

### Handlers (t_handler)

Each handler is a function that knows how to process one specific type of argument.

#### Examples:

- `handle_int` → prints signed integers
- `handle_uint` → prints unsigned integers
- `handle_str` → prints strings
- `handle_ptr` → prints pointer addresses

All handlers share the same function signature (`int handler(va_list *args)`), so they can be stored uniformly in the dictionary.

## Instructions

### Build the library

Run:

```
make
```

This compiles all source files into object files and archives them into a static library, `libftprintf.a`.

### Using the library

We first implement a main.c function to test `ft_printf`.

#### main.c
```
#include "ft_printf.h"

int main(void)
{
    ft_printf("Hello %s!\n", "world");
    ft_printf("Character: %c\n", 'A');
    ft_printf("Decimal: %d\n", 42);
    ft_printf("Unsigned: %u\n", 3000000000u);
    ft_printf("Hex lowercase: %x\n", 255);
    ft_printf("Hex uppercase: %X\n", 255);
    ft_printf("Pointer: %p\n", (void *)&main);
    ft_printf("Percent sign: %%\n");

    return (0);
}
```

Then run the following command:
```
cc -Wall -Wextra -Werror -o my_program main.c libftprintf.a
```

Then, to run test our `main.c` function:
```
./my_program
```
## Resources
- [How to create functions with a variable number of arguments using stdarg.h | C Programming Tutorial](https://www.youtube.com/watch?v=3iX9a_l9W9Y)