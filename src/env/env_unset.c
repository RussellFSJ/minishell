/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:53:50 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_unset(t_env **env, const char *key)
{
	t_env	*target;
	t_env	*current;

	if (!env || !key)
		return (0);
	target = env_find(*env, key);
	if (!target)
		return (0);
	if (target == *env)
		*env = target->next;
	else
	{
		current = *env;
		while (current->next != target)
			current = current->next;
		current->next = target->next;
	}
	free(target->key);
	free(target->value);
	free(target);
	return (0);
}
