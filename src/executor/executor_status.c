/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_status.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/09/12 22:04:47 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	status_collect(t_shell *shell, int last_pid)
{
	int	status;

	if (waitpid(last_pid, &status, 0) == -1)
		return (-1);
	shell->last_status = status_decode(status);
	return (shell->last_status);
}

int	status_decode(int wait_status)
{
	if (WIFEXITED(wait_status))
		return (WEXITSTATUS(wait_status));
	if (WIFSIGNALED(wait_status))
		return (128 + WTERMSIG(wait_status));
	return (1);
}
