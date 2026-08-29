/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_variable.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:05 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 03:45:44 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

#include "minishell.h"

static int	is_var_start(char c)
{
	return (ft_isalpha(c) || c == '_');
}

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static char	*get_env_value(t_shell *shell, const char *word, int start, int len)
{
	char	*key;
	char	*value;
	char	*result;

	key = ft_substr(word, start, len);
	if (!key)
		return (NULL);
	value = env_get(shell->env, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
	result = ft_strdup(value);
	return (result);
}

char	*expand_variable(t_shell *shell, const char *word, int *pos)
{
	int	start;

	if (!shell || !word || !pos || word[*pos] != '$')
		return (NULL);
	(*pos)++;
	if (word[*pos] == '?')
	{
		(*pos)++;
		return (ft_itoa(shell->last_status));
	}
	if (!is_var_start(word[*pos]))
		return (ft_strdup("$"));
	start = *pos;
	while (is_var_char(word[*pos]))
		(*pos)++;
	return (get_env_value(shell, word, start, *pos - start));
}
