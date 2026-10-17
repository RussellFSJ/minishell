/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_word.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 03:41:15 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	add_word_token(t_token **list, char *value);

int	lex_word(const char *line, int *pos, t_token **list)
{
	int		start;
	char	*value;

	start = *pos;
	while (line[*pos] && !ft_strchr(" \t\n\v\f\r", line[*pos])
		&& !lex_is_operator_start(line[*pos]))
	{
		if (line[*pos] == '\'' || line[*pos] == '"')
		{
			if (lex_quote_scan(line, pos) == -1)
				return (-2);
		}
		else
			(*pos)++;
	}
	value = ft_substr(line, start, *pos - start);
	if (value == NULL)
		return (-1);
	return (add_word_token(list, value));
}

static int	add_word_token(t_token **list, char *value)
{
	t_token	*token;

	token = lex_token_new(TOKEN_WORD, value);
	if (token == NULL)
	{
		free(value);
		return (-1);
	}
	lex_token_add_back(list, token);
	return (0);
}
