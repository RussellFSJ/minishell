/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:53:51 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_command	*parse(t_token *tokens, int *error)
{
	(void)tokens;
	*error = 0;
	return (NULL);
}

t_command	*command_new(void)
{
	return (NULL);
}

void	command_add_back(t_command **list, t_command *node)
{
	(void)list;
	(void)node;
}
