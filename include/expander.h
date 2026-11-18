/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:53 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/19 03:45:42 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

typedef enum e_quote
{
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote;

typedef struct s_expand_ctx
{
	const char	*word;
	char		*expanded;
	size_t		pos;
	t_quote		quote;
}	t_expand_ctx;

typedef struct s_field_ctx
{
	const char	*word;
	char		**fields;
	size_t		pos;
	size_t		field_count;
	t_quote		quote;
	int			field_open;
}	t_field_ctx;

int		expand(t_shell *shell, t_command *cmds);
char	*expand_word(t_shell *shell, const char *word);
int		expand_fields(t_shell *shell, const char *word, char ***fields);
int		expand_field_ensure(t_field_ctx *ctx);
int		expand_field_append_text(t_field_ctx *ctx, const char *text);
int		expand_field_append_unquoted(t_field_ctx *ctx, const char *value);
int		expand_argument(t_shell *shell, const char *word,
			int keep_assignment, char ***fields);
int		expand_export_command(const char *word);
char	*expand_variable(t_shell *shell, const char *word, size_t *pos);
char	*expand_strip_quotes(const char *word);
t_quote	expand_quote_update(char c, t_quote quote);
int		expand_append_text(char **result, const char *text);
int		expand_append_char(char **result, char c);
int		expand_argv(t_shell *shell, char ***argv);
int		expand_redirs(t_shell *shell, t_redir *redir);

#endif
