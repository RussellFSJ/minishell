/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:13 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:52:01 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"

int	builtin_dispatch(t_shell *shell, t_command *cmd)
{
	(void)shell;
	(void)cmd;
	return (-1);
}

int	is_builtin(const char *name)
{
	(void)name;
	return (0);
}
