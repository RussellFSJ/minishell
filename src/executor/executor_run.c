/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_run.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:59 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parent_builtin(t_shell *shell, t_command *cmd);

int	executor_run(t_shell *shell, t_command *cmds)
{
	int	status;

	if (!shell)
		return (1);
	if (!cmds)
		return (shell->last_status);
	status = heredoc_collect(shell, cmds);
	if (status == 0)
	{
		if (!cmds->next && cmds->argv && builtin_is_builtin(cmds->argv[0]))
			status = parent_builtin(shell, cmds);
		else
		{
			signals_setup_wait();
			status = executor_pipeline_run(shell, cmds);
			signals_setup_prompt();
		}
	}
	heredoc_close_fds(cmds, NULL);
	shell->last_status = status;
	return (status);
}

static int	parent_builtin(t_shell *shell, t_command *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return (1);
	}
	status = 1;
	if (redirect_apply(shell, cmd) == 0)
		status = builtin_dispatch(shell, cmd);
	if (dup2(saved_stdin, STDIN_FILENO) == -1)
		status = 1;
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		status = 1;
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}
