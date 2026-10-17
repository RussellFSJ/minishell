/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:13 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 15:38:13 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_dispatch(t_shell *shell, t_command *cmd)
{
	char	*name;

	if (!shell || !cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	name = cmd->argv[0];
	if (ft_strncmp(name, "echo", 5) == 0)
		return (builtin_echo(cmd->argv));
	if (ft_strncmp(name, "cd", 3) == 0)
		return (builtin_cd(shell, cmd->argv));
	if (ft_strncmp(name, "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(name, "export", 7) == 0)
		return (builtin_export(shell, cmd->argv));
	if (ft_strncmp(name, "unset", 6) == 0)
		return (builtin_unset(shell, cmd->argv));
	if (ft_strncmp(name, "env", 4) == 0)
		return (builtin_env(shell));
	if (ft_strncmp(name, "exit", 5) == 0)
		return (builtin_exit(shell, cmd->argv));
	return (-1);
}
