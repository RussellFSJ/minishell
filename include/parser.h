/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:59 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_command	*parser_parse(t_token *tokens, int *error);
t_command	*parser_command_new(void);
void		parser_command_add_back(t_command **list, t_command *node);
int			parser_add_redir(t_command *cmd, t_token *token);
int			parser_check_syntax(t_token *tokens);
int			parser_build_argv(t_command *cmd, t_token *tokens);
int			parser_is_redirection(t_token_type type);

#endif
