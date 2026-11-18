/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/23 08:22:26 by russ1337         ###   ########.fr       */
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
