/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:24 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:45:48 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(t_shell *shell, char **argv)
{
	size_t	i;

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
