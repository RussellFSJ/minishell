/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_output.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_write(const char *text)
{
	return (write_all(STDOUT_FILENO, text, ft_strlen(text)));
}

int	builtin_putline(const char *text)
{
	return (builtin_write(text) || builtin_write("\n"));
}

int	builtin_output_error(const char *name)
{
	int	saved_errno;

	saved_errno = errno;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	errno = saved_errno;
	perror(name);
	return (1);
}
