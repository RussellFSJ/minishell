/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:31 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/14 21:24:33 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_open(t_redir *redir);
static int	redirect_one(t_redir *redir);

int	redirect_apply(t_command *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redirect_one(redir) != 0)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int	redirect_open(t_redir *redir)
{
	if (redir->type == TOKEN_HEREDOC)
	{
		if (redir->fd < 0)
			errno = EBADF;
		return (redir->fd);
	}
	if (redir->type == TOKEN_REDIR_IN)
		return (open(redir->target, O_RDONLY));
	if (redir->type == TOKEN_REDIR_OUT)
		return (open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0666));
	if (redir->type == TOKEN_REDIR_APPEND)
		return (open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0666));
	errno = EINVAL;
	return (-1);
}

static int	redirect_one(t_redir *redir)
{
	int	fd;
	int	target;
	int	result;

	target = STDOUT_FILENO;
	if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
		target = STDIN_FILENO;
	fd = redirect_open(redir);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		return (perror(redir->target), 1);
	}
	result = dup2(fd, target);
	if (result == -1)
		perror("minishell: dup2");
	if (redir->type != TOKEN_HEREDOC && fd != target)
		close(fd);
	return (result == -1);
}
