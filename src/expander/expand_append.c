/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_append_text(char **result, const char *text)
{
	char	*joined;

	joined = ft_strjoin(*result, text);
	if (!joined)
		return (-1);
	free(*result);
	*result = joined;
	return (0);
}

int	expand_append_char(char **result, char c)
{
	char	text[2];

	text[0] = c;
	text[1] = '\0';
	return (expand_append_text(result, text));
}
