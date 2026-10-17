/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:59 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:55:02 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parent_builtin(t_shell *shell, t_command *cmd);
static void	executor_close_heredocs(t_command *cmds);

int	execute(t_shell *shell, t_command *cmds)
{
	int	status;

	if (!shell)
		return (1);
	if (!cmds)
		return (shell->last_status);
	status = heredoc_collect(shell, cmds);
	if (status == 0)
	{
		if (!cmds->next && cmds->argv && is_builtin(cmds->argv[0]))
			status = parent_builtin(shell, cmds);
		else
		{
			signals_setup_wait();
			status = pipeline_run(shell, cmds);
			signals_setup_prompt();
		}
	}
	executor_close_heredocs(cmds);
	shell->last_status = status;
	return (status);
}

static int	parent_builtin(t_shell *shell, t_command *cmd)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
	{
		perror("minishell: dup");
		if (saved_in != -1)
			close(saved_in);
		if (saved_out != -1)
			close(saved_out);
		return (1);
	}
	status = 1;
	if (redirect_apply(cmd) == 0)
		status = builtin_dispatch(shell, cmd);
	if (dup2(saved_in, STDIN_FILENO) == -1)
		status = 1;
	if (dup2(saved_out, STDOUT_FILENO) == -1)
		status = 1;
	close(saved_in);
	close(saved_out);
	return (status);
}

static void	executor_close_heredocs(t_command *cmds)
{
	t_redir	*redir;

	while (cmds)
	{
		redir = cmds->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->fd >= 0)
			{
				close(redir->fd);
				redir->fd = -1;
			}
			redir = redir->next;
		}
		cmds = cmds->next;
	}
}
