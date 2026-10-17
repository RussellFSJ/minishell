/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 04:58:32 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_redirs(t_shell *shell, t_redir *redir)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			expanded = expand_strip_quotes(redir->target);
		else
			expanded = expand_word(shell, redir->target);
		if (!expanded)
			return (-1);
		free(redir->target);
		redir->target = expanded;
		redir = redir->next;
	}
	return (0);
}
