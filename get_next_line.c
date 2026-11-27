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

char    *update_storage(char *storage)
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

char	*extract_line(char *storage)
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

char    *get_next_line(int fd)
{
	static char	*storage;
	char		buffer[BUFFER_SIZE + 1];
	char		*line;
	char		*tmp;
	ssize_t		bytes_read;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (storage != NULL && ft_strchr(storage, '\n'))
	{
		line = extract_line(storage); 
		storage = update_storage(storage); 
		return (line);
	}
	while (1)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(storage);
			storage = NULL;
			return (NULL);
		}
		if (bytes_read == 0)
			break;
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(storage, buffer);
		free(storage);
		storage = tmp;
		if (!storage)
			return (NULL);
		if (ft_strchr(storage, '\n'))
			break ;
	}
	if (!storage || storage[0] == '\0')
	{
		free(storage);
		storage = NULL;
		return (NULL);
	}
	line = extract_line(storage);
	storage = update_storage(storage);
	return (line);
}
