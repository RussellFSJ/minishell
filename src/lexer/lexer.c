/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 04:29:10 by ebin-ahm         ###   ########.fr       */
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
	t_token	*list;
	int		pos;

	list = NULL;
	pos = 0;
	*error = 0;
	while (line[pos])
	{
		skip_spaces(line, &pos);
		if (line[pos] == '\0')
			break ;
		if (is_operator_start(line[pos]))
			*error = lex_operator(line, &pos, &list);
		else
			*error = lex_word(line, &pos, &list);
		if (*error == -1)
			return (list);
	}
	return (list);
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
