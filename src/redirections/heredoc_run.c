/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_run.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 03:49:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/15 05:37:25 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	heredoc_child(t_shell *shell, t_redir *redir, int write_fd);
static int	heredoc_wait(pid_t pid);
static int	heredoc_parent_setup(t_heredoc_parent *parent);
static int	heredoc_parent_restore(t_heredoc_parent *parent, int status);

int	heredoc_run(t_shell *shell, t_redir *redir, int write_fd)
{
	t_heredoc_parent	parent;
	pid_t				pid;
	int					status;

	if (heredoc_parent_setup(&parent))
		return (perror("minishell: heredoc signals"), 1);
	pid = fork();
	if (pid == 0)
		heredoc_child(shell, redir, write_fd);
	status = 1;
	if (pid < 0)
		perror("minishell: heredoc fork");
	else
		status = heredoc_wait(pid);
	return (heredoc_parent_restore(&parent, status));
}

static void	heredoc_child(t_shell *shell, t_redir *redir, int write_fd)
{
	int	status;

	signals_setup_heredoc();
	heredoc_close_fds(shell->commands, NULL);
	status = heredoc_read(shell, redir, write_fd);
	close(write_fd);
	executor_child_exit(shell, status);
}

static int	heredoc_wait(pid_t pid)
{
	int	wait_status;

	while (waitpid(pid, &wait_status, 0) < 0)
	{
		if (errno != EINTR)
			return (perror("minishell: heredoc wait"), 1);
	}
	if (WIFSIGNALED(wait_status) && WTERMSIG(wait_status) == SIGINT)
	{
		if (write_all(STDERR_FILENO, "\n", 1))
			perror("minishell: heredoc output");
	}
	return (executor_status_decode(wait_status));
}

static int	heredoc_parent_setup(t_heredoc_parent *parent)
{
	parent->terminal_saved = (tcgetattr(STDIN_FILENO,
				&parent->saved_terminal) == 0);
	parent->old_int = signal(SIGINT, SIG_IGN);
	if (parent->old_int == SIG_ERR)
		return (1);
	parent->old_quit = signal(SIGQUIT, SIG_IGN);
	if (parent->old_quit == SIG_ERR)
	{
		signal(SIGINT, parent->old_int);
		return (1);
	}
	return (0);
}

static int	heredoc_parent_restore(t_heredoc_parent *parent, int status)
{
	if (parent->terminal_saved && tcsetattr(STDIN_FILENO,
			TCSANOW, &parent->saved_terminal) == -1)
	{
		perror("minishell: heredoc terminal");
		if (status != 130)
			status = 1;
	}
	if (signal(SIGINT, parent->old_int) == SIG_ERR && status != 130)
		status = 1;
	if (signal(SIGQUIT, parent->old_quit) == SIG_ERR && status != 130)
		status = 1;
	return (status);
}
