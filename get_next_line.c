#include "get_next_line.h"

// gnl logic
// 1) check if there is something left from the previous call in the static variable
// 2) if there is, check if it contains a newline character
// 3) if it does, return the line up to the newline character and update the static variable
// 4) if it doesn't, read from the file descriptor until a newline character is found or EOF is reached
// 5) if a newline character is found, return the line up to the newline character and update the static variable
// 6) if EOF is reached, return the remaining content in the static variable and free it
// 7) if there is nothing left in the static variable and EOF is reached, return NULL
// 8) handle errors appropriately, such as invalid file descriptors or memory allocation failures

char    *get_next_line(int fd)
{
	static char *storage;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
}
