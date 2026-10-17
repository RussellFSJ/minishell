/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 01:54:10 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	build_redirs(t_command *cmd, t_token *tokens);
static int	build_command(t_command **list, t_token *tokens);

t_command	*parser_parse(t_token *tokens, int *error)
{
	t_command	*cmds;

	if (!error)
		return (NULL);
	*error = 0;
	if (parser_check_syntax(tokens) == -1)
	{
		*error = -2;
		return (NULL);
	}
	cmds = NULL;
	while (tokens)
	{
		if (build_command(&cmds, tokens) == -1)
		{
			free_commands(cmds);
			*error = -1;
			return (NULL);
		}
		while (tokens && tokens->type != TOKEN_PIPE)
			tokens = tokens->next;
		if (tokens)
			tokens = tokens->next;
	}
	return (cmds);
}

static int	build_redirs(t_command *cmd, t_token *tokens)
{
	while (tokens && tokens->type != TOKEN_PIPE)
	{
		if (tokens->type >= TOKEN_REDIR_IN
			&& tokens->type <= TOKEN_HEREDOC)
		{
			if (parser_add_redir(cmd, tokens) == -1)
				return (-1);
		}
		tokens = tokens->next;
	}
	return (0);
}

static int	build_command(t_command **list, t_token *tokens)
{
	t_command	*cmd;

	cmd = parser_command_new();
	if (!cmd)
		return (-1);
	if (parser_build_argv(cmd, tokens) == -1
		|| build_redirs(cmd, tokens) == -1)
	{
		free_commands(cmd);
		return (-1);
	}
	parser_command_add_back(list, cmd);
	return (0);
}
