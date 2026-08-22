/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_dump.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:10 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 04:38:46 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "libft.h"

static char	*token_type_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

void	dump_tokens(t_token *tokens, int fd)
{
	while (tokens != NULL)
	{
		ft_putstr_fd(token_type_name(tokens->type), fd);
		ft_putstr_fd(": ", fd);
		if (tokens->value != NULL)
			ft_putstr_fd(tokens->value, fd);
		ft_putchar_fd('\n', fd);
		tokens = tokens->next;
	}
}
