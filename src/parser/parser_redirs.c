/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:21 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 01:35:05 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_quotes(char *value)
{
	while (*value)
	{
		if (*value == '\'' || *value == '"')
			return (1);
		value++;
	}
	return (0);
}

static t_redir	*redir_new(t_token *token)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->target = ft_strdup(token->next->value);
	if (!redir->target)
	{
		free(redir);
		return (NULL);
	}
	redir->type = token->type;
	redir->expand_body = 0;
	if (token->type == TOKEN_HEREDOC && !has_quotes(token->next->value))
		redir->expand_body = 1;
	redir->fd = -1;
	redir->next = NULL;
	return (redir);
}

static void	redir_add_back(t_redir **list, t_redir *node)
{
	t_redir	*last;

	if (!*list)
	{
		*list = node;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = node;
}

int	parser_add_redir(t_command *cmd, t_token *token)
{
	t_redir	*redir;

	if (!cmd || !token || !token->next)
		return (-1);
	redir = redir_new(token);
	if (!redir)
		return (-1);
	redir_add_back(&cmd->redirs, redir);
	return (0);
}
