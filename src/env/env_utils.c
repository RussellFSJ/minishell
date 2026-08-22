/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:45 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 16:21:51 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

#include "env.h"
#include "libft.h"

t_env	*env_new(char *key, char *value, int exported)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = NULL;
	if (value)
		node->value = ft_strdup(value);
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->exported = exported;
	node->next = NULL;
	return (node);
}

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

