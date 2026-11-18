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
static int	cd_old_pwd(t_shell *shell, char **old_pwd);

int	builtin_cd(t_shell *shell, char **argv)
{
	char	*old_pwd;
	char	*target;
	int		status;

	if (!shell || !argv || !argv[0])
		return (1);
	if (argv[1] && argv[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", 2), 1);
	target = cd_target(shell, argv);
	if (!target)
		return (1);
	if (cd_old_pwd(shell, &old_pwd))
		return (1);
	status = builtin_cd_move(shell, target, old_pwd);
	free(old_pwd);
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

static int	cd_old_pwd(t_shell *shell, char **old_pwd)
{
	char	*value;

	*old_pwd = NULL;
	value = env_get(shell->env, "PWD");
	if (!value)
		return (0);
	*old_pwd = ft_strdup(value);
	if (!*old_pwd)
		return (ft_putendl_fd("minishell: cd: allocation failed", 2), 1);
	return (0);
}
