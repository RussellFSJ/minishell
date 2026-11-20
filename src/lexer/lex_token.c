/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*lex_token_new(t_token_type type, char *value)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	lex_token_add_back(t_token **list, t_token *node)
{
	t_token	*last;

	if (list == NULL || node == NULL )
		return ;
	if (*list == NULL)
	{
		*list = node;
		return ;
	}
	last = *list;
	while (last->next != NULL)
		last = last->next;
	last->next = node;
}

int	lex_is_operator_start(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}
