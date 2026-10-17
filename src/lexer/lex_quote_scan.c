/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_quote_scan.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 03:41:15 by ebin-ahm         ###   ########.fr       */
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
