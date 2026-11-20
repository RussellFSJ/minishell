/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:43 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_add_entry(t_env **env, char *entry);

t_env	*env_init(char **envp)
{
	t_env	*env;
	size_t	i;

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

static int	env_add_entry(t_env **env, char *entry)
{
	char	*equals;
	char	*key;
	t_env	*node;

	equals = ft_strchr(entry, '=');
	if (!equals)
		return (0);
	key = ft_substr(entry, 0, equals - entry);
	if (!key)
		return (-1);
	node = env_new(key, equals + 1, 1);
	free(key);
	if (!node)
		return (-1);
	env_add_back(env, node);
	return (0);
}
