/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/11/19 05:25:24 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

typedef struct s_heredoc_parent
{
	t_signal_handler	old_int;
	t_signal_handler	old_quit;
	struct termios		saved_terminal;
	int					terminal_saved;
}	t_heredoc_parent;

int		redirect_apply(t_shell *shell, t_command *cmd);
int		heredoc_collect(t_shell *shell, t_command *cmds);
int		heredoc_run(t_shell *shell, t_redir *redir, int write_fd);
int		heredoc_read(t_shell *shell, t_redir *redir, int write_fd);
void	heredoc_close_fds(t_command *cmds, t_command *keep_cmd);

#endif
