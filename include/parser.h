/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:59 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/29 23:43:17 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "types.h"

t_command	*parse(t_token *tokens, int *error);
t_command	*command_new(void);
void		command_add_back(t_command **list, t_command *node);
int			parser_add_redir(t_command *cmd, t_token *token);
int			parser_check_syntax(t_token *tokens);
int			parser_build_argv(t_command *cmd, t_token *tokens);

#endif
