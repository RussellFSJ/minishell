/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 16:42:59 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static t_env	*env_find(t_env *env, const char *key)
{
	size_t	key_len;

	if (!key)
		return (NULL);
	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->key, key, key_len + 1) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

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

char	*env_get(t_env *env, const char *key)
{
	t_env	*node;

	node = env_find(env, key);
	if (!node)
		return (NULL);
	return (node->value);
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
