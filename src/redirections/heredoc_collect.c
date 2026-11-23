/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:29 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_name(t_shell *shell);
static int	heredoc_temp(t_shell *shell, int *read_fd);
static int	heredoc_one(t_shell *shell, t_redir *redir);

int	heredoc_collect(t_shell *shell, t_command *cmds)
{
	t_redir	*redir;
	int		status;

	if (!shell)
		return (1);
	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC)
			{
				status = heredoc_one(shell, redir);
				if (status != 0)
					return (status);
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
	return (0);
}

static char	*heredoc_name(t_shell *shell)
{
	char	*number;
	char	*name;

	number = ft_itoa(shell->heredoc_sequence);
	if (!number)
		return (NULL);
	name = ft_strjoin("/tmp/minishell-heredoc-", number);
	free(number);
	if (!name)
		return (NULL);
	if (shell->heredoc_sequence == INT_MAX)
		shell->heredoc_sequence = 0;
	else
		shell->heredoc_sequence++;
	return (name);
}

static int	heredoc_temp(t_shell *shell, int *read_fd)
{
	char		*name;
	int			write_fd;

	write_fd = -1;
	while (write_fd < 0)
	{
		name = heredoc_name(shell);
		if (!name)
			return (-1);
		write_fd = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (write_fd >= 0)
			*read_fd = open(name, O_RDONLY);
		if (write_fd >= 0)
			unlink(name);
		free(name);
		if (write_fd < 0 && errno != EEXIST)
			return (-1);
	}
	return (write_fd);
}

static int	heredoc_one(t_shell *shell, t_redir *redir)
{
	int	write_fd;
	int	status;

	if (redir->fd >= 0)
		close(redir->fd);
	redir->fd = -1;
	write_fd = heredoc_temp(shell, &redir->fd);
	if (write_fd < 0 || redir->fd < 0)
	{
		perror("minishell: heredoc");
		if (write_fd >= 0)
			close(write_fd);
		return (1);
	}
	status = heredoc_run(shell, redir, write_fd);
	close(write_fd);
	return (status);
}
