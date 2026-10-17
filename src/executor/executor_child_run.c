/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_child_run.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:48 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/15 03:50:35 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_error(const char *name, int error);
static void	child_exec(t_shell *shell, t_command *cmd);

void	executor_child_run(t_shell *shell, t_command *cmd)
{
	signals_setup_child();
	if (redirect_apply(cmd) != 0)
		exit(1);
	if (!cmd->argv || !cmd->argv[0])
		exit(0);
	if (builtin_is_builtin(cmd->argv[0]))
		exit(builtin_dispatch(shell, cmd));
	child_exec(shell, cmd);
}

static void	child_error(const char *name, int error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error == ENOENT && !ft_strchr(name, '/'))
	{
		ft_putstr_fd((char *)name, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
	}
	else
	{
		errno = error;
		perror(name);
	}
	if (error == ENOENT || error == ENOTDIR)
		exit(127);
	if (error == ENOMEM)
		exit(1);
	exit(126);
}

static void	child_exec(t_shell *shell, t_command *cmd)
{
	char		*path;
	char		**envp;
	int			error;
	struct stat	info;

	path = executor_path_resolve(shell->env, cmd->argv[0]);
	if (!path)
		child_error(cmd->argv[0], errno);
	envp = env_to_array(shell->env);
	if (!envp)
	{
		free(path);
		child_error(cmd->argv[0], ENOMEM);
	}
	if (stat(path, &info) == 0 && S_ISDIR(info.st_mode))
		error = EISDIR;
	else
	{
		execve(path, cmd->argv, envp);
		error = errno;
	}
	free(path);
	free_array(envp);
	child_error(cmd->argv[0], error);
}
