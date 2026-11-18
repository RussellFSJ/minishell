/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_argv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 03:55:57 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 04:58:32 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	append_arg(char ***argv, size_t *count, char *value)
{
	char	**new_argv;
	size_t	i;

	new_argv = ft_calloc(*count + 2, sizeof(char *));
	if (!new_argv)
		return (-1);
	i = 0;
	while (i < *count)
	{
		new_argv[i] = (*argv)[i];
		i++;
	}
	new_argv[*count] = value;
	free(*argv);
	*argv = new_argv;
	(*count)++;
	return (0);
}

static int	append_fields(char ***argv, size_t *count, char **fields)
{
	size_t	i;

	i = 0;
	while (fields[i])
	{
		if (append_arg(argv, count, fields[i]) == -1)
		{
			while (fields[i])
				free(fields[i++]);
			return (free(fields), -1);
		}
		i++;
	}
	free(fields);
	return (0);
}

static int	expand_arguments(t_shell *shell, char **argv,
		char ***expanded, int is_export)
{
	char	**fields;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (argv[i])
	{
		if (expand_argument(shell, argv[i], is_export && i, &fields) == -1)
			return (-1);
		if (append_fields(expanded, &count, fields) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	expand_argv(t_shell *shell, char ***argv)
{
	char	**expanded;
	int		is_export;

	if (!argv || !*argv)
		return (0);
	is_export = expand_export_command((*argv)[0]);
	if (is_export == -1)
		return (-1);
	expanded = ft_calloc(1, sizeof(char *));
	if (!expanded)
		return (-1);
	if (expand_arguments(shell, *argv, &expanded, is_export) == -1)
		return (free_array(expanded), -1);
	free_array(*argv);
	*argv = expanded;
	return (0);
}
