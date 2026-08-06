/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:50:42 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

char	*env_get(t_env *env, const char *key)
{
	(void)env;
	(void)key;
	return (NULL);
}

int	env_set(t_env **env, const char *key, const char *value)
{
	(void)env;
	(void)key;
	(void)value;
	return (0);
}

int	env_unset(t_env **env, const char *key)
{
	(void)env;
	(void)key;
	return (0);
}
