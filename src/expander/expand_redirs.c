/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/23 19:11:28 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_redirs(t_redir *redir)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			expanded = expand_strip_quotes(redir->target);
			if (!expanded)
				return (-1);
			free(redir->target);
			redir->target = expanded;
		}
		redir = redir->next;
	}
	return (0);
}
