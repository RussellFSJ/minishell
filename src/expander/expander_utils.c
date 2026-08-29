/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:56:11 by ebin-ahm         ###   ########.fr       */
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
