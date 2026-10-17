/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_token_add_back.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:18 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 06:11:42 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
