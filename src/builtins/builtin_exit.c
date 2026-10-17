/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:19 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 19:20:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <limits.h>

static int	exit_parse_status(const char *arg, int *status);
static int	exit_check_overflow(const char *arg, int negative,
			unsigned long long *value);
static int	exit_numeric_error(t_shell *shell, char *arg);

int	builtin_exit(t_shell *shell, char **argv)
{
	int	status;

	if (!shell || !argv || !argv[0])
		return (1);
	if (!argv[1])
	{
		shell->should_exit = 1;
		return (shell->last_status);
	}
	if (!exit_parse_status(argv[1], &status))
		return (exit_numeric_error(shell, argv[1]));
	if (argv[2])
		return (ft_putendl_fd("minishell: exit: too many arguments", 2), 1);
	shell->last_status = status;
	shell->should_exit = 1;
	return (status);
}

static int	exit_parse_status(const char *arg, int *status)
{
	unsigned long long	value;
	int					negative;

	if (!ft_isnumeric(arg))
		return (0);
	negative = (*arg == '-');
	if (*arg == '+' || *arg == '-')
		arg++;
	if (exit_check_overflow(arg, negative, &value))
		return (0);
	*status = (unsigned char)value;
	if (negative)
		*status = (unsigned char)(-*status);
	return (1);
}

static int	exit_check_overflow(const char *arg, int negative,
		unsigned long long *value)
{
	unsigned long long	limit;

	limit = (unsigned long long)LLONG_MAX + negative;
	*value = 0;
	while (*arg)
	{
		if (*value > (limit - (*arg - '0')) / 10)
			return (1);
		*value = *value * 10 + (*arg++ - '0');
	}
	return (0);
}

static int	exit_numeric_error(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putendl_fd(": numeric argument required", 2);
	shell->last_status = 2;
	shell->should_exit = 1;
	return (2);
}
