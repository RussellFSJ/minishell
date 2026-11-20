/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_quote	expand_quote_update(char c, t_quote quote)
{
	if (c == '\'' && quote != QUOTE_DOUBLE)
	{
		if (quote == QUOTE_SINGLE)
			return (QUOTE_NONE);
		if (quote == QUOTE_NONE)
			return (QUOTE_SINGLE);
	}
	if (c == '"' && quote != QUOTE_SINGLE)
	{
		if (quote == QUOTE_DOUBLE)
			return (QUOTE_NONE);
		if (quote == QUOTE_NONE)
			return (QUOTE_DOUBLE);
	}
	return (quote);
}
