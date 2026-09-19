/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:53 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:55:25 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	pipe_child(t_shell *shell, t_command *cmd, int input, int *fd)
{
	if ((input != -1 && dup2(input, STDIN_FILENO) == -1)
		|| (fd[1] != -1 && dup2(fd[1], STDOUT_FILENO) == -1))
	{
		perror("minishell: dup2");
		exit(1);
	}
	if (input != -1)
		close(input);
	if (fd[0] != -1)
		close(fd[0]);
	if (fd[1] != -1)
		close(fd[1]);
	child_run(shell, cmd);
}

static int	pipe_spawn(t_shell *shell, t_command *cmd, int *input)
{
	int	fd[2];
	int	pid;

	fd[0] = -1;
	fd[1] = -1;
	if (cmd->next && pipe(fd) == -1)
		return (perror("minishell: pipe"), -1);
	pid = fork();
	if (pid == 0)
		pipe_child(shell, cmd, *input, fd);
	if (pid == -1)
		perror("minishell: fork");
	if (fd[1] != -1)
		close(fd[1]);
	if (*input != -1)
		close(*input);
	*input = fd[0];
	return (pid);
}

static int	pipeline_wait(t_shell *shell, int *pids, int count, int failed)
{
	int	i;
	int	result;

	result = 1;
	if (count && !failed)
	{
		result = status_collect(shell, pids[count - 1]);
		while (result == -1 && errno == EINTR)
			result = status_collect(shell, pids[count - 1]);
		count--;
	}
	i = 0;
	while (i < count)
	{
		if (failed)
			kill(pids[i], SIGKILL);
		while (waitpid(pids[i], NULL, 0) == -1 && errno == EINTR)
			;
		i++;
	}
	free(pids);
	if (result < 0)
		result = 1;
	shell->last_status = result;
	return (result);
}

static int	command_count(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

int	pipeline_run(t_shell *shell, t_command *cmds)
{
	int	*pids;
	int	count;
	int	input;

	if (!cmds)
		return (shell->last_status);
	pids = malloc(sizeof(int) * command_count(cmds));
	if (!pids)
		return (shell->last_status = 1);
	count = 0;
	input = -1;
	while (cmds)
	{
		pids[count] = pipe_spawn(shell, cmds, &input);
		if (pids[count] == -1)
			break ;
		count++;
		cmds = cmds->next;
	}
	if (input != -1)
		close(input);
	return (pipeline_wait(shell, pids, count, cmds != NULL));
}
