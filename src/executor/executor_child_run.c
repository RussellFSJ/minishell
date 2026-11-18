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

static int	child_error(const char *name, int error);
static int	child_exec(t_shell *shell, t_command *cmd);
static int	child_execve(char *path, char **argv, char **envp);

void	executor_child_run(t_shell *shell, t_command *cmd)
{
	int	status;

	signals_setup_child();
	status = 1;
	if (redirect_apply(shell, cmd) == 0)
	{
		status = 0;
		if (cmd->argv && cmd->argv[0])
		{
			if (builtin_is_builtin(cmd->argv[0]))
				status = builtin_dispatch(shell, cmd);
			else
				status = child_exec(shell, cmd);
		}
	}
	executor_child_exit(shell, status);
}

static int	child_error(const char *name, int error)
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
		return (127);
	if (error == ENOMEM)
		return (1);
	return (126);
}

static int	child_exec(t_shell *shell, t_command *cmd)
{
	char	*path;
	char	**envp;
	int		error;

	path = executor_path_resolve(shell->env, cmd->argv[0]);
	if (!path)
		return (child_error(cmd->argv[0], errno));
	envp = env_to_array(shell->env);
	if (!envp)
	{
		free(path);
		return (child_error(cmd->argv[0], ENOMEM));
	}
	error = child_execve(path, cmd->argv, envp);
	free(path);
	free_array(envp);
	return (child_error(cmd->argv[0], error));
}

static int	child_execve(char *path, char **argv, char **envp)
{
	struct stat	info;

	if (stat(path, &info) == 0 && S_ISDIR(info.st_mode))
		return (EISDIR);
	execve(path, argv, envp);
	return (errno);
}
