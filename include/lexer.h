/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 04:52:49 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "types.h"

t_token	*lex(const char *line, int *error);
t_token	*token_new(t_token_type type, char *value);
void	token_add_back(t_token **list, t_token *node);
int		lex_word(const char *line, int *pos, t_token **list);
int		lex_operator(const char *line, int *pos, t_token **list);
int		quote_scan(const char *line, int *pos);
void	dump_tokens(t_token *tokens, int fd);
int		is_operator_start(char c);

#endif