/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quote_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:03 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:33:36 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_quote_update(char c, int state)
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
