/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argument.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	assignment_word(const char *word)
{
	size_t	i;

	if (!ft_isalpha(word[0]) && word[0] != '_')
		return (0);
	i = 1;
	while (ft_isalnum(word[i]) || word[i] == '_')
		i++;
	return (word[i] == '=');
}

int	expand_export_command(const char *word)
{
	char	*name;
	int		result;

	if (!word)
		return (0);
	name = expand_strip_quotes(word);
	if (!name)
		return (-1);
	result = (ft_strncmp(name, "export", 7) == 0);
	free(name);
	return (result);
}

int	expand_argument(t_shell *shell, const char *word,
		int keep_assignment, char ***fields)
{
	char	**result;

	if (!keep_assignment || !assignment_word(word))
		return (expand_fields(shell, word, fields));
	result = ft_calloc(2, sizeof(char *));
	if (!result)
		return (-1);
	result[0] = expand_word(shell, word);
	if (!result[0])
		return (free(result), -1);
	*fields = result;
	return (0);
}
