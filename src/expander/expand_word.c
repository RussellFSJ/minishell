/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:08 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:03:38 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_dollar(t_shell *shell, t_expand_ctx *ctx);
static int	process_word_char(t_shell *shell, t_expand_ctx *ctx);

char	*expand_word(t_shell *shell, const char *word)
{
	t_expand_ctx	ctx;

	if (!word)
		return (NULL);
	ctx.expanded = ft_strdup("");
	if (!ctx.expanded)
		return (NULL);
	ctx.word = word;
	ctx.pos = 0;
	ctx.quote = QUOTE_NONE;
	while (word[ctx.pos])
	{
		if (process_word_char(shell, &ctx) == -1)
		{
			free(ctx.expanded);
			return (NULL);
		}
	}
	return (ctx.expanded);
}

static int	expand_dollar(t_shell *shell, t_expand_ctx *ctx)
{
	char	*value;

	value = expand_variable(shell, ctx->word, &ctx->pos);
	if (!value)
		return (-1);
	if (expand_append_text(&ctx->expanded, value) == -1)
	{
		free(value);
		return (-1);
	}
	free(value);
	return (0);
}

static int	process_word_char(t_shell *shell, t_expand_ctx *ctx)
{
	t_quote	next_quote;

	next_quote = expand_quote_update(ctx->word[ctx->pos], ctx->quote);
	if (next_quote != ctx->quote)
		ctx->quote = next_quote;
	else if (ctx->word[ctx->pos] == '$' && ctx->quote != QUOTE_SINGLE)
		return (expand_dollar(shell, ctx));
	else if (expand_append_char(&ctx->expanded, ctx->word[ctx->pos]) == -1)
		return (-1);
	ctx->pos++;
	return (0);
}
