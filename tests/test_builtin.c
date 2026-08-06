/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:46 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:59:15 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "types.h"
#include "builtins.h"

int	main(int argc, char **argv)
{
	t_shell		shell;
	t_command	cmd;
	char		*words[2];

	(void)argc;
	(void)argv;
	words[0] = "echo";
	words[1] = NULL;
	cmd.argv = words;
	cmd.redirs = NULL;
	cmd.next = NULL;
	shell.env = NULL;
	shell.last_status = 0;
	shell.should_exit = 0;
	printf("dispatch(echo) -> %d\n", builtin_dispatch(&shell, &cmd));
	return (0);
}
