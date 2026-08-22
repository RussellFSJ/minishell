/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:43 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 16:33:00 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "libft.h"

static int	env_add_entry(t_env **env, char *entry)
{
	char	*equal;
	char	*key;
	t_env	*node;

	equal = ft_strchr(entry, '=');
	if (!equal)
		return (0);
	key = ft_substr(entry, 0, equal - entry);
	if (!key)
		return (-1);
	node = env_new(key, equal + 1, 1);
	free(key);
	if (!node)
		return (-1);
	env_add_back(env, node);
	return (0);
}

t_env	*env_init(char **envp)
{
	t_env	*env;
	int		i;

	env = NULL;
	i = 0;
	while (envp && envp[i])
	{
		if (env_add_entry(&env, envp[i]) == -1)
		{
			free_env(env);
			return (NULL);
		}
		i++;
	}
	return (env);
}

