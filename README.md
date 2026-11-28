*This project has been created as part of the 42 curriculum by asallam.*

# get_next_line

## Description

get_next_line is a function in C that reads a file descriptor and returns one line each time it is called.

```c
char *get_next_line(int fd);
```

If you call it in a loop, you get the whole file line by line. It works with files, stdin and pipes. The goal of the project is to learn how `read()` works, how to manage memory without leaks, and how a static variable keeps its value between calls.

How it behaves:
- it returns the line with the `\n` at the end, except for the last line if the file doesn't end with `\n`
- it returns `NULL` when there is nothing left to read or if there is an error
- it works with any `BUFFER_SIZE`
- it stops reading as soon as it finds a `\n`, it doesn't read the whole file first


Files:
- `get_next_line.c`: the main function and its static helpers
- `get_next_line_utils.c`: ft_strlen, ft_strjoin, ft_strdup, ft_strchr
- `get_next_line.h`: prototypes and the default `BUFFER_SIZE`

## Instructions

Compile with the buffer size you want:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

If `-D BUFFER_SIZE` is not given, the default value in the header is 10.

`main.c` is just my test file, it is not part of the project. Example:

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int	main(void)
{
	int		fd;
	char	*line;

	fd = open("test.txt", O_RDONLY);
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (0);
}
```

The caller has to free every line that is returned. To read from stdin, use fd `0`.

Things I test: different buffer sizes (1, 5, 42, 9999, 10000000), an empty file, a file with only `\n`, a file without a `\n` at the end, empty lines in the middle, stdin, an invalid fd, and calling it again after it returned `NULL`. I also check for leaks with valgrind and run norminette.

## Algorithm

The function keeps what it already read but didn't return yet in a static pointer called `storage`. Since a static variable keeps its value between calls, the next call continues from where the last one stopped.

Each call does three steps:

1. `read_to_storage`: reads `BUFFER_SIZE` bytes at a time and adds them to `storage`, until `storage` has a `\n` or `read` returns 0 (end of file). If `storage` already has a `\n` from before, it doesn't call `read` at all.
2. `extract_line`: copies `storage` into a new string, from the start until the first `\n` (included), or until the end if there is no `\n`. This is the line that gets returned.
3. `update_storage`: replaces `storage` with what is left after the line. If nothing is left, `storage` is freed and set to `NULL`.

Example:

```
storage = "wor"                (left from the last call)
read gives "ld\nfo"            storage = "world\nfo", it has a \n so we stop
extract_line returns "world\n"
update_storage keeps "fo"      for the next call
```
- the read buffer is allocated with `malloc` and freed in every case (error, end of file, normal return). I did not use an array on the stack because a `BUFFER_SIZE` like 10000000 would crash it.
- `ft_strjoin` creates a new string, so the old `storage` is freed right after the join and the result is checked for `NULL`.
- if `read` fails or a `malloc` fails, `storage` is freed and set to `NULL`, so the next call starts clean.

- a static variable is the way to keep the state between calls without using a global variable, which is forbidden
- stopping at the first `\n` follows the rule of reading as little as possible, and it also works with stdin because it doesn't wait for the end of the input
- one string for `storage` keeps the code simple and fits in the Norm limits (25 lines per function, 5 functions per file)
- it works with any `BUFFER_SIZE`: with 1 the loop just runs more times, with a big value one `read` can bring many lines and the rest stays in `storage` for the next calls

## Resources

- [Static variables in C (GeeksforGeeks)](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptor (Wikipedia)](https://en.wikipedia.org/wiki/File_descriptor)
- [Valgrind manual](https://valgrind.org/docs/manual/manual.html)