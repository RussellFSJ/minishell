/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_body.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/15 03:49:27 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/15 03:49:28 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	heredoc_write(int fd, const char *text);
static int	heredoc_piece(t_shell *shell, char *line, int *i, int fd);
static int	heredoc_expand(t_shell *shell, char *line, int fd);
static int	heredoc_line(t_shell *shell, t_redir *redir, char *line, int fd);

int	heredoc_read(t_shell *shell, t_redir *redir, int fd)
{
	char	*line;
	int		status;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc ended before ", 2);
			ft_putendl_fd(redir->target, 2);
			return (0);
		}
		if (ft_strcmp(line, redir->target) == 0)
			return (free(line), 0);
		status = heredoc_line(shell, redir, line, fd);
		free(line);
		if (status)
			return (perror("minishell: heredoc"), 1);
	}
}

static int	heredoc_write(int fd, const char *text)
{
	ssize_t	written;
	size_t	length;

	length = ft_strlen(text);
	while (length)
	{
		written = write(fd, text, length);
		if (written < 0 && errno == EINTR)
			continue ;
		if (written <= 0)
			return (1);
		text += written;
		length -= written;
	}
	return (0);
}

static int	heredoc_piece(t_shell *shell, char *line, int *i, int fd)
{
	char	*value;
	char	text[2];
	int		status;

	if (line[*i] == '$')
	{
		value = expand_variable(shell, line, i);
		if (!value)
			return (1);
		status = heredoc_write(fd, value);
		free(value);
		return (status);
	}
	if (line[*i] == '\\' && (line[*i + 1] == '$'
			|| line[*i + 1] == '\\' || line[*i + 1] == '`'))
		(*i)++;
	text[0] = line[(*i)++];
	text[1] = '\0';
	return (heredoc_write(fd, text));
}

static int	heredoc_expand(t_shell *shell, char *line, int fd)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (heredoc_piece(shell, line, &i, fd))
			return (1);
	}
	return (0);
}

static int	heredoc_line(t_shell *shell, t_redir *redir, char *line, int fd)
{
	int	status;

	if (redir->expand_body)
		status = heredoc_expand(shell, line, fd);
	else
		status = heredoc_write(fd, line);
	if (!status)
		status = heredoc_write(fd, "\n");
	return (status);
}
