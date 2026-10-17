/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:10 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 17:17:29 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*cd_target(t_shell *shell, char **argv);
static int	cd_update_env(t_shell *shell, char *current_dir);

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*current_dir;
	char	*target;
	int		status;

	if (!shell || !argv || !argv[0])
		return (1);
	if (argv[1] && argv[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	target = cd_target(shell, argv);
	if (!target)
		return (1);
	current_dir = getcwd(NULL, 0);
	if (!current_dir)
		return (perror("minishell: cd"), 1);
	status = 1;
	if (chdir(target) == -1)
		perror("minishell: cd");
	else
		status = cd_update_env(shell, current_dir);
	free(current_dir);
	return (status);
}

static char	*cd_target(t_shell *shell, char **argv)
{
	char	*target;

	if (argv[1])
		return (argv[1]);
	target = env_get(shell->env, "HOME");
	if (!target)
		ft_putendl_fd("minishell: cd: HOME not set", 2);
	return (target);
}

static int	cd_update_env(t_shell *shell, char *current_dir)
{
	char	*new_dir;
	int		status;

	new_dir = getcwd(NULL, 0);
	if (!new_dir)
	{
		perror("minishell: cd");
		return (1);
	}
	status = 0;
	if (env_set(&shell->env, "OLDPWD", current_dir) != 0
		|| env_set(&shell->env, "PWD", new_dir) != 0)
		status = 1;
	if (status)
		perror("minishell: cd");
	free(new_dir);
	return (status);
}
