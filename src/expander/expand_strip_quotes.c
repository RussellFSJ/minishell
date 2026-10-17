/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_strip_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:33:36 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_strip_quotes(const char *word)
{
	char	*result;
	size_t	i;
	int		j;
	int		state;
	int		next;

	if (word == NULL)
		return (NULL);
	result = malloc(ft_strlen(word) + 1);
	if (result == NULL)
		return (NULL);
	i = 0;
	j = 0;
	state = 0;
	while (word[i])
	{
		next = expand_quote_update(word[i], state);
		if (next == state)
			result[j++] = word[i];
		state = next;
		i++;
	}
	result[j] = '\0';
	return (result);
}
