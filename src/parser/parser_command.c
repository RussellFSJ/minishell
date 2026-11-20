/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_command.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parser_command_new(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	parser_command_add_back(t_command **list, t_command *node)
{
	t_command	*last;

	if (!list || !node)
		return ;
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

int	parser_is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}
