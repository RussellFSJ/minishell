/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 01:54:10 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	cmd = command_new();
	if (!cmd)
		return (-1);
	if (parser_build_argv(cmd, tokens) == -1
		|| build_redirs(cmd, tokens) == -1)
	{
		free_commands(cmd);
		return (-1);
	}
	command_add_back(list, cmd);
	return (0);
}

t_command	*parse(t_token *tokens, int *error)
{
	t_command	*cmds;

	if (!error)
		return (NULL);
	*error = 0;
	if (parser_check_syntax(tokens) == -1)
	{
		*error = -1;
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

t_command	*command_new(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

void	command_add_back(t_command **list, t_command *node)
{
	t_command	*last;

	if (!list || !node)
		return ;
	if (!*list)
	{
		*list = node;
		return ;
	}
	last = *list;
	while (last->next)
		last = last->next;
	last->next = node;
}
