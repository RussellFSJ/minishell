/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:51:09 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token	*lex(const char *line, int *error)
{
	(void)line;
	*error = 0;
	return (NULL);
}

t_token	*token_new(t_token_type type, char *value)
{
	(void)type;
	(void)value;
	return (NULL);
}

void	token_add_back(t_token **list, t_token *node)
{
	(void)list;
	(void)node;
}
