/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:08 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:52:38 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	expand_dollar(t_shell *shell, const char *word,
		int *i, char **result)
{
	char	*value;

	value = expand_variable(shell, word, i);
	if (!value)
		return (-1);
	if (append_text(result, value) == -1)
	{
		free(value);
		return (-1);
	}
	free(value);
	return (0);
}

static int	process_word_char(t_shell *shell, const char *word,
		int *i, int *state, char **result)
{
	int	next;

	next = quote_update(word[*i], *state);
	if (next != *state)
		*state = next;
	else if (word[*i] == '$' && *state != 1)
		return (expand_dollar(shell, word, i, result));
	else if (append_char(result, word[*i]) == -1)
		return (-1);
	(*i)++;
	return (0);
}

char	*expand_word(t_shell *shell, const char *word)
{
	char	*result;
	int		i;
	int		state;

	if (!word)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	state = 0;
	while (word[i])
	{
		if (process_word_char(shell, word, &i, &state, &result) == -1)
		{
			free(result);
			return (NULL);
		}
	}
	return (result);
}

int	expand(t_shell *shell, t_command *cmds)
{
	(void)shell;
	(void)cmds;
	return (0);
}
