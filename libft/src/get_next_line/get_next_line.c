/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 22:10:45 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:24 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*handle_free(char *stash, char *buffer);
static char	*process_line(char *stash);
static char	*update_stash(char *stash);

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	ssize_t		bytes_read;
	char		*line;

	buffer = malloc(BUFFER_SIZE + 1);
	if (fd < 0 || BUFFER_SIZE <= 0 || !buffer)
		return (handle_free(stash, buffer));
	bytes_read = 0;
	while (!stash || !ft_strchr(stash, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read < 0)
			return (handle_free(stash, buffer));
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		stash = ft_strjoin(stash, buffer);
	}
	if (!stash || *stash == '\0')
		return (handle_free(stash, buffer));
	line = process_line(stash);
	stash = update_stash(stash);
	free(buffer);
	return (line);
}

static char	*handle_free(char *stash, char *buffer)
{
	if (stash)
	{
		free(stash);
		stash = NULL;
	}
	free(buffer);
	return (NULL);
}

static char	*process_line(char *stash)
{
	int		i;
	char	*line;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (stash[i] == '\n')
		i++;
	line = malloc(i + 1);
	if (!line)
	{
		free(stash);
		return (NULL);
	}
	ft_memcpy(line, stash, i);
	line[i] = '\0';
	return (line);
}

static char	*update_stash(char *stash)
{
	int		i;
	size_t	length;
	char	*new_stash;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
	{
		free(stash);
		return (NULL);
	}
	i++;
	length = ft_strlen(stash + i);
	new_stash = malloc(length + 1);
	if (!new_stash)
	{
		free(stash);
		return (NULL);
	}
	ft_memcpy(new_stash, stash + i, length);
	new_stash[length] = '\0';
	free(stash);
	return (new_stash);
}
