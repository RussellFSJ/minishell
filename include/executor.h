/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:50 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 20:05:57 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "types.h"

int		execute(t_shell *shell, t_command *cmds);
int		pipeline_run(t_shell *shell, t_command *cmds);
char	*path_resolve(t_env *env, const char *name);
void	child_run(t_shell *shell, t_command *cmd);
int		status_collect(t_shell *shell, int last_pid);
int		status_decode(int wait_status);
int		redirect_apply(t_command *cmd);
int		heredoc_collect(t_shell *shell, t_command *cmds);
int		heredoc_run(t_shell *shell, t_redir *redir, int output);
int		heredoc_read(t_shell *shell, t_redir *redir, int fd);

#endif
