/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fields.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 04:02:58 by russ1337          #+#    #+#             */
/*   Updated: 2026/11/19 04:03:12 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_quote(t_field_ctx *ctx);
static int	process_variable(t_shell *shell, t_field_ctx *ctx);
static int	process_char(t_shell *shell, t_field_ctx *ctx);

int	expand_fields(t_shell *shell, const char *word, char ***fields)
{
	t_field_ctx	ctx;

	if (!shell || !word || !fields)
		return (-1);
	ctx.word = word;
	ctx.fields = ft_calloc(1, sizeof(char *));
	ctx.pos = 0;
	ctx.field_count = 0;
	ctx.quote = QUOTE_NONE;
	ctx.field_open = 0;
	if (!ctx.fields)
		return (-1);
	while (ctx.word[ctx.pos])
	{
		if (process_char(shell, &ctx) == -1)
			return (free_array(ctx.fields), -1);
	}
	*fields = ctx.fields;
	return (0);
}

static int	process_quote(t_field_ctx *ctx)
{
	t_quote	next_quote;

	next_quote = expand_quote_update(ctx->word[ctx->pos], ctx->quote);
	if (next_quote == ctx->quote)
		return (0);
	ctx->quote = next_quote;
	ctx->pos++;
	if (expand_field_ensure(ctx) == -1)
		return (-1);
	return (1);
}

static int	process_variable(t_shell *shell, t_field_ctx *ctx)
{
	char	*value;
	int		status;

	value = expand_variable(shell, ctx->word, &ctx->pos);
	if (!value)
		return (-1);
	if (ctx->quote == QUOTE_DOUBLE)
		status = expand_field_append_text(ctx, value);
	else
		status = expand_field_append_unquoted(ctx, value);
	free(value);
	return (status);
}

static int	process_char(t_shell *shell, t_field_ctx *ctx)
{
	char	text[2];
	int		status;

	status = process_quote(ctx);
	if (status == -1)
		return (-1);
	if (status == 1)
		return (0);
	if (ctx->word[ctx->pos] == '$' && ctx->quote != QUOTE_SINGLE)
		return (process_variable(shell, ctx));
	text[0] = ctx->word[ctx->pos++];
	text[1] = '\0';
	return (expand_field_append_text(ctx, text));
}
