/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_syntax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:24 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/29 23:30:55 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redirection(t_token_type type)
{
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT)
		return (1);
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

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
		if (is_redirection(tokens->type)
			&& (tokens->next == NULL || tokens->next->type != TOKEN_WORD))
			return (-1);
		tokens = tokens->next;
	}
	return (0);
}
