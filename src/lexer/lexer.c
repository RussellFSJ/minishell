/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 01:50:58 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static void skip_spaces(const char *line, int *pos)
{
	while (line[*pos] && ft_isspace(line[*pos]))
		(*pos)++;
}

t_token	*lex(const char *line, int *error)
{
	(void)line;
	*error = 0;
	return (NULL);
}

t_token	*token_new(t_token_type type, char *value)
{
	t_token *node;

	node = malloc(sizeof(t_token));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->value = value;
	node->next = NULL;
	return (node);
}

void	token_add_back(t_token **list, t_token *node)
{
	t_token *last;

	if (list == NULL || node == NULL )
		return;
	if (*list == NULL)
	{
		*list = node;
		return;
	}
	last = *list;
	while (last->next != NULL)
		last = last->next;
	last->next = node;
}
