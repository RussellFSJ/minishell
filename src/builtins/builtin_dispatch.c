/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:13 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:25:47 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(const char *name)
{
	if (!name)
		return (0);
	if (ft_strcmp(name, "echo") == 0
		|| ft_strcmp(name, "cd") == 0
		|| ft_strcmp(name, "pwd") == 0
		|| ft_strcmp(name, "export") == 0
		|| ft_strcmp(name, "unset") == 0
		|| ft_strcmp(name, "env") == 0
		|| ft_strcmp(name, "exit") == 0)
		return (1);
	return (0);
}

int	builtin_dispatch(t_shell *shell, t_command *cmd)
{
	char	*name;

	if (!shell || !cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	name = cmd->argv[0];
	if (ft_strcmp(name, "echo") == 0)
		return (builtin_echo(shell, cmd->argv));
	if (ft_strcmp(name, "cd") == 0)
		return (builtin_cd(shell, cmd->argv));
	if (ft_strcmp(name, "pwd") == 0)
		return (builtin_pwd(shell, cmd->argv));
	if (ft_strcmp(name, "export") == 0)
		return (builtin_export(shell, cmd->argv));
	if (ft_strcmp(name, "unset") == 0)
		return (builtin_unset(shell, cmd->argv));
	if (ft_strcmp(name, "env") == 0)
		return (builtin_env(shell, cmd->argv));
	return (builtin_exit(shell, cmd->argv));
}
