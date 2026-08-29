/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:53 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:03:22 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "types.h"

typedef struct s_expand_ctx
{
	const char	*word;
	char		*result;
	int			i;
	int			state;
}	t_expand_ctx;

int		expand(t_shell *shell, t_command *cmds);
char	*expand_word(t_shell *shell, const char *word);
char	*expand_variable(t_shell *shell, const char *word, int *pos);
char	*strip_quotes(const char *word);
int		quote_update(char c, int state);
int		append_text(char **result, const char *text);
int		append_char(char **result, char c);
int		expand_argv(t_shell *shell, char **argv);
int		expand_redirs(t_shell *shell, t_redir *redir);

#endif
