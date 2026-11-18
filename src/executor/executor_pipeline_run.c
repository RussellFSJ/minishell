/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipeline_run.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:53 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:55:25 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void		pipe_child(t_shell *shell, t_command *cmd,
					int input_fd, int *pipe_fds);
static int		pipe_spawn(t_shell *shell, t_command *cmd, int *input_fd,
					t_command *head);
static int		pipeline_wait(t_shell *shell, int *pids,
					size_t child_count, int spawn_failed);
static size_t	command_count(t_command *cmd);

int	executor_pipeline_run(t_shell *shell, t_command *cmds)
{
	int			*pids;
	size_t		child_count;
	int			input_fd;
	t_command	*head;

	if (!cmds)
		return (shell->last_status);
	pids = malloc(sizeof(int) * command_count(cmds));
	if (!pids)
		return (shell->last_status = 1);
	shell->pids = pids;
	child_count = 0;
	input_fd = -1;
	head = cmds;
	while (cmds)
	{
		pids[child_count] = pipe_spawn(shell, cmds, &input_fd, head);
		if (pids[child_count] == -1)
			break ;
		child_count++;
		cmds = cmds->next;
	}
	if (input_fd != -1)
		close(input_fd);
	return (pipeline_wait(shell, pids, child_count, cmds != NULL));
}

static void	pipe_child(t_shell *shell, t_command *cmd,
		int input_fd, int *pipe_fds)
{
	int	error;

	error = 0;
	if ((input_fd != -1 && dup2(input_fd, STDIN_FILENO) == -1)
		|| (pipe_fds[1] != -1 && dup2(pipe_fds[1], STDOUT_FILENO) == -1))
		error = errno;
	if (input_fd != -1 && input_fd != STDIN_FILENO)
		close(input_fd);
	if (pipe_fds[0] != -1)
		close(pipe_fds[0]);
	if (pipe_fds[1] != -1 && pipe_fds[1] != STDOUT_FILENO)
		close(pipe_fds[1]);
	if (error)
	{
		errno = error;
		perror("minishell: dup2");
		executor_child_exit(shell, 1);
	}
	executor_child_run(shell, cmd);
}

static int	pipe_spawn(t_shell *shell, t_command *cmd, int *input_fd,
		t_command *head)
{
	int	pipe_fds[2];
	int	pid;

	pipe_fds[0] = -1;
	pipe_fds[1] = -1;
	if (cmd->next && pipe(pipe_fds) == -1)
		return (perror("minishell: pipe"), -1);
	pid = fork();
	if (pid == 0)
	{
		heredoc_close_fds(head, cmd);
		pipe_child(shell, cmd, *input_fd, pipe_fds);
	}
	if (pid == -1)
		perror("minishell: fork");
	if (pipe_fds[1] != -1)
		close(pipe_fds[1]);
	if (*input_fd != -1)
		close(*input_fd);
	*input_fd = pipe_fds[0];
	return (pid);
}

static int	pipeline_wait(t_shell *shell, int *pids,
		size_t child_count, int spawn_failed)
{
	size_t	i;
	int		result;

	result = 1;
	if (child_count && !spawn_failed)
	{
		result = executor_status_collect(shell, pids[child_count - 1]);
		while (result == -1 && errno == EINTR)
			result = executor_status_collect(shell, pids[child_count - 1]);
		child_count--;
	}
	i = 0;
	while (i < child_count)
	{
		if (spawn_failed)
			kill(pids[i], SIGKILL);
		while (waitpid(pids[i], NULL, 0) == -1 && errno == EINTR)
			;
		i++;
	}
	free(pids);
	shell->pids = NULL;
	if (result < 0)
		result = 1;
	return (shell->last_status = result);
}

static size_t	command_count(t_command *cmd)
{
	size_t	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}
