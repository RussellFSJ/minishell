/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/19 03:58:29 by russ1337         ###   ########.fr       */
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
