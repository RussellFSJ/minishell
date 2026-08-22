/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 03:41:15 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"
#include <stdlib.h>

static int	add_word_token(t_token **list, char *value)
{
	t_token	*token;

	token = token_new(TOKEN_WORD, value);
	if (token == NULL)
	{
		free(value);
		return (-1);
	}
	token_add_back(list, token);
	return (0);
}

int	lex_word(const char *line, int *pos, t_token **list)
{
	int		start;
	char	*value;

	start = *pos;
	while (line[*pos] && !ft_isspace(line[*pos])
		&& !is_operator_start(line[*pos]))
	{
		if (line[*pos] == '\'' || line[*pos] == '"')
		{
			if (quote_scan(line, pos) == -1)
				return (-1);
		}
		else
			(*pos)++;
	}
	value = ft_substr(line, start, *pos - start);
	if (value == NULL)
		return (-1);
	return (add_word_token(list, value));
}

int	quote_scan(const char *line, int *pos)
{
	char	quote;

	quote = line[*pos];
	(*pos)++;
	while (line[*pos] && line[*pos] != quote)
		(*pos)++;
	if (line[*pos] == '\0')
		return (-1);
	(*pos)++;
	return (0);
}
