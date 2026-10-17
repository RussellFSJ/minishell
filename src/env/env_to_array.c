/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_to_array.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:36 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:53:57 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	env_count_exported(t_env *env);
static char		*env_make_entry(t_env *env);
static char		**env_free_partial(char **array, size_t count);

char	**env_to_array(t_env *env)
{
	char	**array;
	size_t	i;

	array = malloc(sizeof(char *) * (env_count_exported(env) + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (env)
	{
		if (env->exported && env->value)
		{
			array[i] = env_make_entry(env);
			if (!array[i])
				return (env_free_partial(array, i));
			i++;
		}
		env = env->next;
	}
	array[i] = NULL;
	return (array);
}

static size_t	env_count_exported(t_env *env)
{
	size_t	count;

	count = 0;
	while (env)
	{
		if (env->exported && env->value)
			count++;
		env = env->next;
	}
	return (count);
}

static char	*env_make_entry(t_env *env)
{
	char	*prefix;
	char	*entry;

	prefix = ft_strjoin(env->key, "=");
	if (!prefix)
		return (NULL);
	entry = ft_strjoin(prefix, env->value);
	free(prefix);
	return (entry);
}

static char	**env_free_partial(char **array, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}
