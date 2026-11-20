/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

t_token	*lex(const char *line, int *error);
t_token	*lex_token_new(t_token_type type, char *value);
void	lex_token_add_back(t_token **list, t_token *node);
int		lex_word(const char *line, int *pos, t_token **list);
int		lex_operator(const char *line, int *pos, t_token **list);
int		lex_quote_scan(const char *line, int *pos);
int		lex_is_operator_start(char c);

#endif
