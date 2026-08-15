_This project has been created as part of the 42 curriculum by rfoo._

# get_next_line

## Description
In this project, we will implement `get_next_line`, a function that reads a file descriptor line by line. Each call returns the next line, including the newline character (`\n`),  if present, until the end of the file (EOF) or an error occurs.

This project also teaches us about `static variables` and how it can be used to preserve its value across separate function calls.

### get_next_line_utils


| Function     | Description                                                  			  |
|--------------|----------------------------------------------------------------------|
| `ft_strchr`  | Finds the first occurrence of a character in a string; used to detect `\n`. |
| `ft_strlen`  | Returns the length of a string; used for memory allocation and sizing. |
| `ft_memcpy`  | Copies a fixed number of bytes from source to destination.            |
| `ft_strdup`  | Allocates and duplicates a string; used to initialise or handle `NULL`. |
| `ft_strjoin` | Concatenates two strings into a new allocation; used to append buffer to stash. |

### get_next_line

| Function       | Description                                                                 |
|----------------|-------------------------------------------------------------------------------------|
| `get_next_line` | Reads from a file descriptor, accumulates data in stash, and returns the next line until EOF. |
| `process_line` | Extracts the next line from the stash, including `\n` if present, and returns it.   |
| `update_stash` | Removes the extracted line from the stash, keeping only the leftover for next call. |

## Instructions

Create `file.txt` and implement `main.c` to test `get_next_line` accordingly.

### `file.txt`
```
echo -n "Hello\\nWorld\\nFrom\\n42 SG\\n" > file.txt
```

### `main.c` (from `file.txt`)

```
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
	int		fd;
	char 	*line;
	int 	line_no;

	fd = open("file.txt", O_RDONLY);
	line_no = 1;
	if (fd == -1)
	{
		printf("Error opening file.txt");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("Line %d: %s\n", line_no, line);
		line_no++;
		free(line);
	}
	close(fd);
	printf("EOF reached on file.txt\n");
	return (0);
}
```

### `main.c` (from `stdin`)

```
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
	int		fd;
	char	*line;
	int		line_no;

	fd = 0;
	line_no = 1;
	while ((line = get_next_line(fd)) != NULL) 
	{ 
		printf("Line %d: %s\n", line_no, line);
		line_no++;
		free(line);
	}
	printf("EOF reached on stdin\n");
	return (0);
}
```
Then run the following command:
```
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -o my_program *c
```

Then, to run test our `main.c` function:
```
./my_program
```

## Resources

- [Static Local Variables | C Programming Tutorial](https://www.youtube.com/watch?v=Hm5Jc7ajZsk)