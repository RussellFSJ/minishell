/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 04:58:32 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	append_text(char **result, const char *text)
{
	char	*joined;

	joined = ft_strjoin(*result, text);
	if (!joined)
		return (-1);
	free(*result);
	*result = joined;
	return (0);
}

int	append_char(char **result, char c)
{
	char	text[2];

	text[0] = c;
	text[1] = '\0';
	return (append_text(result, text));
}

int	expand_argv(t_shell *shell, char **argv)
{
	char	*expanded;
	int		i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
	{
		expanded = expand_word(shell, argv[i]);
		if (!expanded)
			return (-1);
		free(argv[i]);
		argv[i] = expanded;
		i++;
	}
	return (0);
}

int	expand_redirs(t_shell *shell, t_redir *redir)
{
	char	*expanded;

	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			expanded = strip_quotes(redir->target);
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
