/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_check_syntax.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:24 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parser_check_syntax(t_token *tokens)
{
	if (tokens == NULL)
		return (0);
	if (tokens->type == TOKEN_PIPE)
		return (-1);
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE
			&& (tokens->next == NULL || tokens->next->type == TOKEN_PIPE))
			return (-1);
		if (parser_is_redirection(tokens->type)
			&& (tokens->next == NULL || tokens->next->type != TOKEN_WORD))
			return (-1);
		tokens = tokens->next;
	}
	return (0);
}
