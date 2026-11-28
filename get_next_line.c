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

static char	*update_storage(char *storage)
{
    char *new_storage;
	char *temp;

	temp = ft_strchr(storage, '\n');
	if (!temp || !temp[1])
	{
		free(storage);
		return (NULL);
	}
	new_storage = ft_strdup(temp + 1); // might return null
	free(storage);
	return (new_storage);
}

static char	*extract_line(char *storage)
{
	char *line;
	size_t i;

	i = 0;
	line = malloc(ft_strlen(storage) + 1);
	if (!line)
		return (NULL);
	while (storage[i] && storage[i] != '\n')
	{
		line[i] = storage[i];
		i++;
	}
	if (storage[i] == '\n')
	{
		line[i] = storage[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char *free_both(char *a, char *b)
{
	free(a);
	free(b);
	return (NULL);
}

static char *read_to_storage(int fd, char *storage)
{
	char	*buffer;
	char	*temp;
	ssize_t	bytes_read;

	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
		return (free_both(NULL, storage));
	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(storage, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (free_both(buffer, storage));
		buffer[bytes_read] = '\0';
		temp = ft_strjoin(storage, buffer);
		free(storage);
		storage = temp;
		if (!storage)
			return (free_both(buffer, NULL));
	}
	free(buffer);
	return (storage);
}

char    *get_next_line(int fd)
{
	static char	*storage;
	char	*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	storage = read_to_storage(fd, storage);
	if (!storage)
		return (NULL);
	line = NULL;
	if (*storage)
		line = extract_line(storage);
	if (!line)
	{
		storage = free_both(storage, NULL);
		return (NULL);
	}
	storage = update_storage(storage);
	return (line);
}
