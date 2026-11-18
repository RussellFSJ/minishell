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
static int		export_print_value(const char *value);
static int		export_print_node(t_env *node);

int	builtin_export_print(t_env *env)
{
	t_env	*node;

	node = export_next(env, NULL);
	while (node)
	{
		if (export_print_node(node))
			return (builtin_output_error("export"));
		node = export_next(env, node);
	}
	return (0);
}

static int	export_print_node(t_env *node)
{
	if (builtin_write("declare -x ") || builtin_write(node->key))
		return (1);
	if (node->value)
	{
		if (builtin_write("=\"") || export_print_value(node->value)
			|| builtin_write("\""))
			return (1);
	}
	return (builtin_write("\n"));
}

static int	export_print_value(const char *value)
{
	char	text[2];

	text[1] = '\0';
	while (*value)
	{
		if (*value == '"' || *value == '\\' || *value == '$'
			|| *value == '`')
		{
			if (builtin_write("\\"))
				return (1);
		}
		text[0] = *value++;
		if (builtin_write(text))
			return (1);
	}
	return (0);
}

static t_env	*export_next(t_env *env, t_env *previous)
{
	t_env	*candidate;

	candidate = NULL;
	while (env)
	{
		if (env->exported
			&& (!previous || ft_strncmp(env->key, previous->key,
					ft_strlen(env->key) + 1) > 0)
			&& (!candidate || ft_strncmp(env->key, candidate->key,
					ft_strlen(env->key) + 1) < 0))
			candidate = env;
		env = env->next;
	}
	return (candidate);
}
