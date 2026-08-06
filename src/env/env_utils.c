/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:45 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:50:57 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"

t_env	*env_new(char *key, char *value, int exported)
{
	(void)key;
	(void)value;
	(void)exported;
	return (NULL);
}

void	env_add_back(t_env **env, t_env *node)
{
	(void)env;
	(void)node;
}
