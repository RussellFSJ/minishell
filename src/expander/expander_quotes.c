/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_quotes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:33:36 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

#include "minishell.h"

int	quote_update(char c, int state)
{
	if (c == '\'' && state != 2)
	{
		if (state == 1)
			return (0);
		if (state == 0)
			return (1);
	}
	if (c == '"' && state != 1)
	{
		if (state == 2)
			return (0);
		if (state == 0)
			return (2);
	}
	return (state);
}

char	*strip_quotes(const char *word)
{
	char	*result;
	int		i;
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
		next = quote_update(word[i], state);
		if (next == state)
			result[j++] = word[i];
		state = next;
		i++;
	}
	result[j] = '\0';
	return (result);
}
