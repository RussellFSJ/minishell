/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/29 23:33:51 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse(t_token *tokens, int *error)
{
	(void)tokens;
	*error = 0;
	return (NULL);
}

t_command	*command_new(void)
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

void	command_add_back(t_command **list, t_command *node)
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
