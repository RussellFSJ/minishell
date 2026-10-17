/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_add_back.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 16:42:59 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_add_back(t_env **env, t_env *node)
{
	t_env	*current;

	if (!env || !node)
		return ;
	if (!*env)
	{
		*env = node;
		return ;
	}
	current = *env;
	while (current->next)
		current = current->next;
	current->next = node;
}
