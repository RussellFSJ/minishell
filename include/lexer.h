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

/* Error codes: 0 success, -1 allocation failure, -2 syntax error. */
t_token	*lex(const char *line, int *error);
t_token	*lex_token_new(t_token_type type, char *value);
void	lex_token_add_back(t_token **list, t_token *node);
int		lex_word(const char *line, int *pos, t_token **list);
int		lex_operator(const char *line, int *pos, t_token **list);
int		lex_quote_scan(const char *line, int *pos);
void	lex_dump_tokens(t_token *tokens, int fd);
int		lex_is_operator_start(char c);

#endif
