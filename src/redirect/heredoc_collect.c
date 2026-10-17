/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:29 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:58:08 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*heredoc_name(void);
static int	heredoc_temp(int *input);
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

static char	*heredoc_name(void)
{
	static int	sequence;
	char		*number;
	char		*name;

	number = ft_itoa(sequence++);
	if (!number)
		return (NULL);
	name = ft_strjoin("/tmp/minishell-heredoc-", number);
	free(number);
	return (name);
}

static int	heredoc_temp(int *input)
{
	char		*name;
	int			output;

	output = -1;
	while (output < 0)
	{
		name = heredoc_name();
		if (!name)
			return (-1);
		output = open(name, O_WRONLY | O_CREAT | O_EXCL, 0600);
		if (output >= 0)
			*input = open(name, O_RDONLY);
		if (output >= 0)
			unlink(name);
		free(name);
		if (output < 0 && errno != EEXIST)
			return (-1);
	}
	return (output);
}

static int	heredoc_one(t_shell *shell, t_redir *redir)
{
	int	output;
	int	status;

	if (redir->fd >= 0)
		close(redir->fd);
	redir->fd = -1;
	output = heredoc_temp(&redir->fd);
	if (output < 0 || redir->fd < 0)
	{
		perror("minishell: heredoc");
		if (output >= 0)
			close(output);
		return (1);
	}
	status = heredoc_run(shell, redir, output);
	close(output);
	return (status);
}
