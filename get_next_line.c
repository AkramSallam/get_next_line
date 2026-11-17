#include "get_next_line.h"

char    *get_next_line(int fd)
{
	static char *storage;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
}
