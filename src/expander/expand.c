/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:08 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/23 19:11:55 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	expand(t_shell *shell, t_command *cmds)
{
	if (!shell)
		return (-1);
	while (cmds)
	{
		if (expand_argv(shell, &cmds->argv) == -1)
			return (-1);
		if (expand_redirs(cmds->redirs) == -1)
			return (-1);
		cmds = cmds->next;
	}
	return (0);
}
