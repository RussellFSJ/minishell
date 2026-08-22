/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_operators.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:13 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 03:33:27 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	is_operator_start(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);
	return (0);
}

int	lex_operator(const char *line, int *pos, t_token **list)
{
	(void)line;
	(void)pos;
	(void)list;
	return (0);
}
