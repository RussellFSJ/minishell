/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cwd_initialize(t_shell *shell)
{
	char	*initial_pwd;

	initial_pwd = env_get(shell->env, "PWD");
	if (cwd_matches(initial_pwd))
		shell->cwd = ft_strdup(initial_pwd);
	else
		shell->cwd = getcwd(NULL, 0);
	if (!shell->cwd && errno == ENOMEM)
		return (1);
	if (shell->cwd && env_set(&shell->env, "PWD", shell->cwd) != 0)
		return (1);
	if (!env_find(shell->env, "OLDPWD"))
		return (env_set(&shell->env, "OLDPWD", NULL) != 0);
	return (0);
}

int	cwd_matches(const char *path)
{
	struct stat	current;
	struct stat	candidate;

	if (!path || path[0] != '/')
		return (0);
	if (stat(".", &current) == -1 || stat(path, &candidate) == -1)
		return (0);
	return (current.st_dev == candidate.st_dev
		&& current.st_ino == candidate.st_ino);
}

char	*cwd_current(t_shell *shell)
{
	if (cwd_matches(shell->cwd))
		return (ft_strdup(shell->cwd));
	return (getcwd(NULL, 0));
}

char	*cwd_target(t_shell *shell, const char *target)
{
	char	*current;
	char	*prefix;
	char	*path;
	char	*result;

	if (!target[0])
		return (cwd_current(shell));
	if (target[0] == '/')
		return (cwd_normalize(target));
	current = cwd_current(shell);
	if (!current && errno != ENOMEM)
		return (ft_strdup(target));
	if (!current)
		return (NULL);
	prefix = ft_strjoin(current, "/");
	free(current);
	if (!prefix)
		return (NULL);
	path = ft_strjoin(prefix, target);
	free(prefix);
	if (!path)
		return (NULL);
	result = cwd_normalize(path);
	return (free(path), result);
}
