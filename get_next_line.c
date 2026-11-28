#include "get_next_line.h"

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
	new_storage = ft_strdup(temp + 1); 
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
// hello
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
