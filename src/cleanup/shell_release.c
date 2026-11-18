/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_release.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	shell_release(t_shell *shell)
{
	heredoc_close_fds(shell->commands, NULL);
	free_commands(shell->commands);
	free_env(shell->env);
	free(shell->cwd);
	free(shell->line);
	free(shell->pids);
	rl_clear_history();
	shell->commands = NULL;
	shell->env = NULL;
	shell->cwd = NULL;
	shell->line = NULL;
	shell->pids = NULL;
}

void	executor_child_exit(t_shell *shell, int status)
{
	shell_release(shell);
	exit(status);
}
