/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd_move.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:10 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 17:17:29 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_set_paths(t_shell *shell, char *old_dir, char *new_dir);
static int	cd_destination(t_shell *shell, const char *target, char **new_dir);
static int	cd_bind_path(t_shell *shell, const char *key, const char *value);

int	builtin_cd_move(t_shell *shell, const char *target, char *old_dir)
{
	char	*new_dir;

	if (cd_destination(shell, target, &new_dir))
		return (perror("minishell: cd"), 1);
	if (new_dir && new_dir[0] != '/')
	{
		free(new_dir);
		new_dir = getcwd(NULL, 0);
	}
	if (!new_dir)
		perror("minishell: cd");
	free(shell->cwd);
	shell->cwd = new_dir;
	return (cd_set_paths(shell, old_dir, new_dir));
}

static int	cd_destination(t_shell *shell, const char *target, char **new_dir)
{
	*new_dir = cwd_target(shell, target);
	if (!*new_dir && errno == ENOMEM)
		return (1);
	if (*new_dir && chdir(*new_dir) == 0)
		return (0);
	free(*new_dir);
	*new_dir = NULL;
	if (chdir(target) == -1)
		return (1);
	*new_dir = getcwd(NULL, 0);
	return (0);
}

static int	cd_set_paths(t_shell *shell, char *old_dir, char *new_dir)
{
	int	status;

	status = (new_dir == NULL);
	if (cd_bind_path(shell, "OLDPWD", old_dir))
		status = 1;
	if (!new_dir)
		env_unset(&shell->env, "PWD");
	else if (cd_bind_path(shell, "PWD", new_dir))
		status = 1;
	if (status)
		ft_putendl_fd("minishell: cd: environment update failed", 2);
	return (status);
}

static int	cd_bind_path(t_shell *shell, const char *key, const char *value)
{
	t_env	*node;
	int		exported;

	node = env_find(shell->env, key);
	exported = 0;
	if (node)
		exported = node->exported;
	if (env_set(&shell->env, key, value) != 0)
		return (env_unset(&shell->env, key), 1);
	node = env_find(shell->env, key);
	node->exported = exported;
	return (0);
}
