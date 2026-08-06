/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:44 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:48:36 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLEANUP_H
# define CLEANUP_H

# include "types.h"

void	free_tokens(t_token *tokens);
void	free_commands(t_command *cmds);
void	free_redirs(t_redir *redirs);
void	free_array(char **array);

#endif
