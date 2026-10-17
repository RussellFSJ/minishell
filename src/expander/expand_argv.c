/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 04:58:32 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand_argv(t_shell *shell, char **argv)
{
	char	*expanded;
	size_t	i;

	if (!argv)
		return (0);
	i = 0;
	while (argv[i])
	{
		expanded = expand_word(shell, argv[i]);
		if (!expanded)
			return (-1);
		free(argv[i]);
		argv[i] = expanded;
		i++;
	}
	return (0);
}
