/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:27 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/29 23:21:06 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands(t_command *cmds)
{
	t_command	*next;

	while (cmds)
	{
		next = cmds->next;
		free_array(cmds->argv);
		free_redirs(cmds->redirs);
		free(cmds);
		cmds = next;
	}
}
