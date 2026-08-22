/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:13 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 04:02:23 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

#include "lexer.h"
#include "libft.h"
#include <stdlib.h>

int	is_operator_start(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

static t_token_type	operator_type(const char *line, int pos, int *len)
{
	*len = 1;
	if (line[pos] == '|')
		return (TOKEN_PIPE);
	if (line[pos] == '<' && line[pos + 1] == '<')
	{
		*len = 2;
		return (TOKEN_HEREDOC);
	}
	if (line[pos] == '>' && line[pos + 1] == '>')
	{
		*len = 2;
		return (TOKEN_REDIR_APPEND);
	}
	if (line[pos] == '<')
		return (TOKEN_REDIR_IN);
	return (TOKEN_REDIR_OUT);
}

int	lex_operator(const char *line, int *pos, t_token **list)
{
	t_token_type	type;
	t_token			*token;
	char			*value;
	int				len;

	type = operator_type(line, *pos, &len);
	value = ft_substr(line, *pos, len);
	if (value == NULL)
		return (-1);
	token = token_new(type, value);
	if (token == NULL)
	{
		free(value);
		return (-1);
	}
	token_add_back(list, token);
	*pos += len;
	return (0);
}
