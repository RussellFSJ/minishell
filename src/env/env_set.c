/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:53:45 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_replace_value(t_env *node, const char *value)
{
	char	*new_value;

	new_value = NULL;
	if (value)
	{
		new_value = ft_strdup(value);
		if (!new_value)
			return (-1);
	}
	free(node->value);
	node->value = new_value;
	node->exported = 1;
	return (0);
}

int	env_set(t_env **env, const char *key, const char *value)
{
	t_env	*node;

	if (!env || !key)
		return (-1);
	node = env_find(*env, key);
	if (node)
		return (env_replace_value(node, value));
	node = env_new((char *)key, (char *)value, 1);
	if (!node)
		return (-1);
	env_add_back(env, node);
	return (0);
}
