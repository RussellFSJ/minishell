/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_print.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:21 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 20:42:55 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*export_next(t_env *env, t_env *previous);

int	builtin_export_print(t_env *env)
{
	t_env	*node;

	node = export_next(env, NULL);
	while (node)
	{
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(node->key, 1);
		if (node->value)
		{
			ft_putstr_fd("=\"", 1);
			ft_putstr_fd(node->value, 1);
			ft_putchar_fd('"', 1);
		}
		ft_putchar_fd('\n', 1);
		node = export_next(env, node);
	}
	return (0);
}

static t_env	*export_next(t_env *env, t_env *previous)
{
	t_env	*next;

	next = NULL;
	while (env)
	{
		if (env->exported
			&& (!previous || ft_strncmp(env->key, previous->key,
					ft_strlen(env->key) + 1) > 0)
			&& (!next || ft_strncmp(env->key, next->key,
					ft_strlen(env->key) + 1) < 0))
			next = env;
		env = env->next;
	}
	return (next);
}
