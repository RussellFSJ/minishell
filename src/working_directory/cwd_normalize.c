/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd_normalize.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	path_component(char *result, const char *part, size_t capacity)
{
	struct stat	info;
	char		*slash;

	if (ft_strncmp(part, ".", 2) == 0)
		return (0);
	if (ft_strncmp(part, "..", 3) == 0)
	{
		if (stat(result, &info) == -1)
			return (-1);
		if (!S_ISDIR(info.st_mode))
			return (errno = ENOTDIR, -1);
		slash = ft_strrchr(result, '/');
		if (slash == result)
			slash++;
		*slash = '\0';
		return (0);
	}
	if (result[1])
		ft_strlcat(result, "/", capacity);
	ft_strlcat(result, part, capacity);
	return (0);
}

char	*cwd_normalize(const char *path)
{
	char	**parts;
	char	*result;
	size_t	i;
	size_t	capacity;

	capacity = ft_strlen(path) + 2;
	parts = ft_split(path, '/');
	if (!parts)
		return (NULL);
	result = ft_calloc(capacity, 1);
	if (!result)
		return (free_array(parts), NULL);
	result[0] = '/';
	i = 0;
	while (parts[i])
	{
		if (path_component(result, parts[i++], capacity) == -1)
			return (free_array(parts), free(result), NULL);
	}
	free_array(parts);
	return (result);
}
