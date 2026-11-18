/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_apply.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:31 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/14 21:24:33 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	redirect_open(t_redir *redir, const char *target);
static int	redirect_one(t_shell *shell, t_redir *redir);
static int	redirect_target(t_shell *shell, t_redir *redir,
				char ***fields, char **path);
static int	redirect_open_error(char *path, char **fields);

int	redirect_apply(t_shell *shell, t_command *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return (0);
	redir = cmd->redirs;
	while (redir)
	{
		if (redirect_one(shell, redir) != 0)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int	redirect_open(t_redir *redir, const char *target)
{
	if (redir->type == TOKEN_HEREDOC)
	{
		if (redir->fd < 0)
			errno = EBADF;
		return (redir->fd);
	}
	if (redir->type == TOKEN_REDIR_IN)
		return (open(target, O_RDONLY));
	if (redir->type == TOKEN_REDIR_OUT)
		return (open(target, O_WRONLY | O_CREAT | O_TRUNC, 0666));
	if (redir->type == TOKEN_REDIR_APPEND)
		return (open(target, O_WRONLY | O_CREAT | O_APPEND, 0666));
	errno = EINVAL;
	return (-1);
}

static int	redirect_one(t_shell *shell, t_redir *redir)
{
	char	**fields;
	char	*path;
	int		source_fd;
	int		dup_result;
	int		target_fd;

	fields = NULL;
	if (redirect_target(shell, redir, &fields, &path))
		return (1);
	source_fd = redirect_open(redir, path);
	if (source_fd < 0)
		return (redirect_open_error(path, fields));
	target_fd = STDOUT_FILENO;
	if (redir->type == TOKEN_REDIR_IN || redir->type == TOKEN_HEREDOC)
		target_fd = STDIN_FILENO;
	dup_result = dup2(source_fd, target_fd);
	if (dup_result == -1)
		perror("minishell: dup2");
	if (source_fd != target_fd)
		close(source_fd);
	if (redir->type == TOKEN_HEREDOC)
		redir->fd = -1;
	free_array(fields);
	return (dup_result == -1);
}

static int	redirect_target(t_shell *shell, t_redir *redir,
			char ***fields, char **path)
{
	*path = redir->target;
	if (redir->type == TOKEN_HEREDOC)
		return (0);
	if (expand_fields(shell, redir->target, fields) == -1)
	{
		ft_putendl_fd("minishell: memory allocation failed", 2);
		return (1);
	}
	if (!(*fields)[0] || (*fields)[1])
	{
		free_array(*fields);
		ft_putendl_fd("minishell: ambiguous redirect", 2);
		return (1);
	}
	*path = (*fields)[0];
	return (0);
}

static int	redirect_open_error(char *path, char **fields)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(path);
	free_array(fields);
	return (1);
}
