/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/23 22:04:47 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor_status_collect(pid_t last_pid)
{
	int	wait_status;

	if (waitpid(last_pid, &wait_status, 0) == -1)
		return (-1);
	return (executor_status_decode(wait_status));
}

int	executor_status_decode(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	if (WIFSIGNALED(wait_status))
	{
		if (WTERMSIG(wait_status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (128 + WTERMSIG(wait_status));
	}
	return (1);
}
