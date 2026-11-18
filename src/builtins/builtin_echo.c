/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:02:26 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_n_option(const char *arg);

int	builtin_echo(char **argv)
{
	size_t	i;
	int		print_newline;

	i = 1;
	print_newline = 1;
	while (argv[i] && is_n_option(argv[i]))
	{
		print_newline = 0;
		i++;
	}
	while (argv[i])
	{
		if (builtin_write(argv[i])
			|| (argv[i + 1] && builtin_write(" ")))
			return (builtin_output_error("echo"));
		i++;
	}
	if (print_newline && builtin_write("\n"))
		return (builtin_output_error("echo"));
	return (0);
}

static int	is_n_option(const char *arg)
{
	size_t	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}
