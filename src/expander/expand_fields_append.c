/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_fields_append.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 04:02:58 by russ1337          #+#    #+#             */
/*   Updated: 2026/11/19 04:03:12 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_field_ensure(t_field_ctx *ctx)
{
	char	**new_fields;
	size_t	i;

	if (ctx->field_open)
		return (0);
	new_fields = ft_calloc(ctx->field_count + 2, sizeof(char *));
	if (!new_fields)
		return (-1);
	i = 0;
	while (i < ctx->field_count)
	{
		new_fields[i] = ctx->fields[i];
		i++;
	}
	new_fields[ctx->field_count] = ft_strdup("");
	if (!new_fields[ctx->field_count])
		return (free(new_fields), -1);
	free(ctx->fields);
	ctx->fields = new_fields;
	ctx->field_count++;
	ctx->field_open = 1;
	return (0);
}

int	expand_field_append_text(t_field_ctx *ctx, const char *text)
{
	char	*joined;

	if (expand_field_ensure(ctx) == -1)
		return (-1);
	joined = ft_strjoin(ctx->fields[ctx->field_count - 1], text);
	if (!joined)
		return (-1);
	free(ctx->fields[ctx->field_count - 1]);
	ctx->fields[ctx->field_count - 1] = joined;
	return (0);
}

int	expand_field_append_unquoted(t_field_ctx *ctx, const char *value)
{
	char	text[2];
	size_t	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == ' ' || value[i] == '\t' || value[i] == '\n')
			ctx->field_open = 0;
		else
		{
			text[0] = value[i];
			text[1] = '\0';
			if (expand_field_append_text(ctx, text) == -1)
				return (-1);
		}
		i++;
	}
	return (0);
}
