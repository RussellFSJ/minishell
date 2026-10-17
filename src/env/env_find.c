/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_find.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 16:42:59 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_find(t_env *env, const char *key)
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
