/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 06:11:42 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_spaces(const char *line, int *pos);

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
		if (lex_is_operator_start(line[pos]))
			*error = lex_operator(line, &pos, &list);
		else
			*error = lex_word(line, &pos, &list);
		if (*error != 0)
		{
			free_tokens(list);
			return (NULL);
		}
	}
	return (list);
}

static void	skip_spaces(const char *line, int *pos)
{
	while (line[*pos] && ft_strchr(" \t\n\v\f\r", line[*pos]))
		(*pos)++;
}
