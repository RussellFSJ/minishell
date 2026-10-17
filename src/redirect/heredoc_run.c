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

static void	heredoc_child(t_shell *shell, t_redir *redir, int output);
static int	heredoc_wait(pid_t pid);
static int	heredoc_ignore(t_signal_handler *old_int,
				t_signal_handler *old_quit);

int	heredoc_run(t_shell *shell, t_redir *redir, int output)
{
	t_signal_handler	old_int;
	t_signal_handler	old_quit;
	pid_t				pid;
	int					status;

	if (heredoc_ignore(&old_int, &old_quit))
		return (perror("minishell: heredoc signals"), 1);
	pid = fork();
	if (pid == 0)
		heredoc_child(shell, redir, output);
	status = 1;
	if (pid < 0)
		perror("minishell: heredoc fork");
	else
		status = heredoc_wait(pid);
	if (signal(SIGINT, old_int) == SIG_ERR)
		status = 1;
	if (signal(SIGQUIT, old_quit) == SIG_ERR)
		status = 1;
	return (status);
}

static void	heredoc_child(t_shell *shell, t_redir *redir, int output)
{
	int	status;

	signals_setup_heredoc();
	close(redir->fd);
	status = heredoc_read(shell, redir, output);
	close(output);
	exit(status);
}

static int	heredoc_wait(pid_t pid)
{
	int	status;

	while (waitpid(pid, &status, 0) < 0)
	{
		if (errno != EINTR)
			return (perror("minishell: heredoc wait"), 1);
	}
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		write(STDERR_FILENO, "\n", 1);
	return (executor_status_decode(status));
}

static int	heredoc_ignore(t_signal_handler *old_int,
		t_signal_handler *old_quit)
{
	*old_int = signal(SIGINT, SIG_IGN);
	if (*old_int == SIG_ERR)
		return (1);
	*old_quit = signal(SIGQUIT, SIG_IGN);
	if (*old_quit == SIG_ERR)
	{
		signal(SIGINT, *old_int);
		return (1);
	}
	return (0);
}
