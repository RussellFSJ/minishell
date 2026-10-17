/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token_new.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 06:11:42 by ebin-ahm         ###   ########.fr       */
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
