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

int	executor_status_collect(pid_t pid, int *interrupted)
{
	int	wait_status;

	while (waitpid(pid, &wait_status, 0) == -1)
	{
		if (errno != EINTR)
			return (1);
	}
	if (WIFSIGNALED(wait_status) && (WTERMSIG(wait_status) == SIGINT
			|| WTERMSIG(wait_status) == SIGQUIT))
		*interrupted = 1;
	return (executor_status_decode(wait_status));
}

int	executor_status_decode(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	if (WIFSIGNALED(wait_status))
		return (128 + WTERMSIG(wait_status));
	return (1);
}
