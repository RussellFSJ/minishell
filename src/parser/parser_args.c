/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/29 23:41:44 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/29 23:42:24 by ebin-ahm         ###   ########.fr       */
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

static int	count_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redirection(tokens->type))
			tokens = tokens->next;
		else
			count++;
		if (tokens)
			tokens = tokens->next;
	}
	return (count);
}

static int	fill_argv(char **argv, t_token *tokens)
{
	int	i;

	i = 0;
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (is_redirection(tokens->type))
			tokens = tokens->next;
		else
		{
			argv[i] = ft_strdup(tokens->value);
			if (!argv[i++])
				return (-1);
		}
		if (tokens)
			tokens = tokens->next;
	}
	return (0);
}

int	parser_build_argv(t_command *cmd, t_token *tokens)
{
	int	count;

	count = count_args(tokens);
	if (count == 0)
		return (0);
	cmd->argv = ft_calloc(count + 1, sizeof(char *));
	if (!cmd->argv)
		return (-1);
	if (fill_argv(cmd->argv, tokens) == -1)
	{
		free_array(cmd->argv);
		cmd->argv = NULL;
		return (-1);
	}
	return (0);
}
