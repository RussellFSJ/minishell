/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:24 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 15:34:13 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_shell *shell)
{
	t_env	*env;

	if (!shell)
		return (1);
	env = shell->env;
	while (env)
	{
		if (env->exported && env->value)
		{
			if (builtin_write(env->key) || builtin_write("=")
				|| builtin_putline(env->value))
				return (builtin_output_error("env"));
		}
		env = env->next;
	}
	return (0);
}
