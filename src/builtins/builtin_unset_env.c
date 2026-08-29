/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:24 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:27:03 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

#include "minishell.h"

int	builtin_unset(t_shell *shell, char **argv)
{
	int	i;

	if (!shell || !argv)
		return (1);
	i = 1;
	while (argv[i])
	{
		if (env_unset(&shell->env, argv[i]) == -1)
			return (1);
		i++;
	}
	return (0);
}

int	builtin_env(t_shell *shell, char **argv)
{
	t_env	*env;

	(void)argv;
	if (!shell)
		return (1);
	env = shell->env;
	while (env)
	{
		if (env->exported && env->value)
		{
			ft_putstr_fd(env->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(env->value, 1);
		}
		env = env->next;
	}
	return (0);
}
