/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_status_collect.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/09/12 22:04:47 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	executor_status_collect(t_shell *shell, int last_pid)
{
	int	status;

	if (waitpid(last_pid, &status, 0) == -1)
		return (-1);
	shell->last_status = executor_status_decode(status);
	return (shell->last_status);
}
