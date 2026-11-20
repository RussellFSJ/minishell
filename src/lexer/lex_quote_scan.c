/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quote_scan.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	lex_quote_scan(const char *line, int *pos)
{
	char	quote;

	quote = line[*pos];
	(*pos)++;
	while (line[*pos] && line[*pos] != quote)
		(*pos)++;
	if (line[*pos] == '\0')
		return (-1);
	(*pos)++;
	return (0);
}
