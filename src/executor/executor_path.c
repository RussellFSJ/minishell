/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:50 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/15 03:50:06 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*path_candidate(const char *path, size_t len, const char *name)
{
	char	*candidate;
	size_t	size;

	size = ft_strlen(name);
	candidate = malloc(len + size + 2);
	if (!candidate)
		return (NULL);
	ft_memcpy(candidate, path, len);
	if (len)
		candidate[len++] = '/';
	ft_memcpy(candidate + len, name, size + 1);
	return (candidate);
}

static char	*path_search(const char *path, const char *name, int *error)
{
	char		*candidate;
	size_t		len;
	struct stat	info;

	while (1)
	{
		len = 0;
		while (path[len] && path[len] != ':')
			len++;
		candidate = path_candidate(path, len, name);
		if (!candidate)
			return (*error = ENOMEM, NULL);
		errno = 0;
		if (stat(candidate, &info) == 0 && !S_ISDIR(info.st_mode)
			&& access(candidate, X_OK) == 0)
			return (candidate);
		if (errno == EACCES)
			*error = EACCES;
		free(candidate);
		if (!path[len])
			return (NULL);
		path += len + 1;
	}
}

char	*path_resolve(t_env *env, const char *name)
{
	char	*path;
	char	*result;
	int		error;

	if (!name || !*name)
		return (errno = ENOENT, NULL);
	if (ft_strchr(name, '/'))
		return (ft_strdup(name));
	path = env_get(env, "PATH");
	if (!path)
		return (ft_strdup(name));
	error = ENOENT;
	result = path_search(path, name, &error);
	if (!result)
		errno = error;
	return (result);
}
