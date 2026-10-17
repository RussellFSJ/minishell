/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_append_text.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 04:58:32 by ebin-ahm         ###   ########.fr       */
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
