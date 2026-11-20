/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_strip_quotes.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_strip_quotes(const char *word)
{
	char	*result;
	size_t	read_pos;
	int		write_pos;
	t_quote	quote;
	t_quote	next_quote;

	if (word == NULL)
		return (NULL);
	result = malloc(ft_strlen(word) + 1);
	if (result == NULL)
		return (NULL);
	read_pos = 0;
	write_pos = 0;
	quote = QUOTE_NONE;
	while (word[read_pos])
	{
		next_quote = expand_quote_update(word[read_pos], quote);
		if (next_quote == quote)
			result[write_pos++] = word[read_pos];
		quote = next_quote;
		read_pos++;
	}
	result[write_pos] = '\0';
	return (result);
}
