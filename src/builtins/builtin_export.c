/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:21 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 20:42:55 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_valid_key(const char *key);
static int	export_parse_arg(t_shell *shell, char *arg);
static int	export_assign(t_shell *shell, char *key, char *value);

int	builtin_export(t_shell *shell, char **argv)
{
	size_t	i;
	int		status;

	if (!shell || !argv || !argv[0])
		return (1);
	if (!argv[1])
		return (builtin_export_print(shell->env));
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (export_parse_arg(shell, argv[i]))
			status = 1;
		i++;
	}
	return (status);
}

static int	export_valid_key(const char *key)
{
	size_t	i;

	if (!key || !key[0])
		return (0);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static int	export_parse_arg(t_shell *shell, char *arg)
{
	char	*equal;
	char	*key;
	char	*value;
	int		status;

	if (!shell || !arg)
		return (1);
	equal = ft_strchr(arg, '=');
	if (equal)
		key = ft_substr(arg, 0, equal - arg);
	else
		key = ft_strdup(arg);
	if (!key)
		return (1);
	value = NULL;
	if (equal)
		value = equal + 1;
	status = export_assign(shell, key, value);
	return (free(key), status);
}

static int	export_assign(t_shell *shell, char *key, char *value)
{
	t_env	*node;

	if (!export_valid_key(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(key, 2);
		ft_putendl_fd("': not a valid identifier", 2);
		return (1);
	}
	node = env_find(shell->env, key);
	if (!value && node)
	{
		node->exported = 1;
		return (0);
	}
	return (env_set(&shell->env, key, value) != 0);
}
