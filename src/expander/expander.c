/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:08 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:03:38 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_dollar(t_shell *shell, t_expand_ctx *ctx)
{
	char	*value;

	value = expand_variable(shell, ctx->word, &ctx->i);
	if (!value)
		return (-1);
	if (append_text(&ctx->result, value) == -1)
	{
		free(value);
		return (-1);
	}
	free(value);
	return (0);
}

static int	process_word_char(t_shell *shell, t_expand_ctx *ctx)
{
	int	next;

	next = quote_update(ctx->word[ctx->i], ctx->state);
	if (next != ctx->state)
		ctx->state = next;
	else if (ctx->word[ctx->i] == '$' && ctx->state != 1)
		return (expand_dollar(shell, ctx));
	else if (append_char(&ctx->result, ctx->word[ctx->i]) == -1)
		return (-1);
	ctx->i++;
	return (0);
}

char	*expand_word(t_shell *shell, const char *word)
{
	t_expand_ctx	ctx;

	if (!word)
		return (NULL);
	ctx.result = ft_strdup("");
	if (!ctx.result)
		return (NULL);
	ctx.word = word;
	ctx.i = 0;
	ctx.state = 0;
	while (word[ctx.i])
	{
		if (process_word_char(shell, &ctx) == -1)
		{
			free(ctx.result);
			return (NULL);
		}
	}
	return (ctx.result);
}

int	expand(t_shell *shell, t_command *cmds)
{
	if (!shell)
		return (-1);
	while (cmds)
	{
		if (expand_argv(shell, cmds->argv) == -1)
			return (-1);
		if (expand_redirs(shell, cmds->redirs) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}
