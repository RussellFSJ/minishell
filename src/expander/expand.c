/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:08 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:03:38 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(t_shell *shell, t_command *cmds)
{
	if (!shell)
		return (-1);
	while (cmds)
	{
		if (expand_argv(shell, cmds->argv) == -1)
			return (-1);
		if (expand_redirs(shell, cmds->redirs) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}
