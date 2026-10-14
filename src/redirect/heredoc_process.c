/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 03:49:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/15 03:49:02 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (status_decode(status));
}

static int	heredoc_ignore(struct sigaction *old_int,
		struct sigaction *old_quit)
{
	struct sigaction	ignore;

	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	sigemptyset(&ignore.sa_mask);
	if (sigaction(SIGINT, &ignore, old_int) < 0)
		return (1);
	if (sigaction(SIGQUIT, &ignore, old_quit) < 0)
		return (sigaction(SIGINT, old_int, NULL), 1);
	return (0);
}

int	heredoc_run(t_shell *shell, t_redir *redir, int output)
{
	struct sigaction	old_int;
	struct sigaction	old_quit;
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
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	return (status);
}
