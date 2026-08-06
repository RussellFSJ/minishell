/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:59:21 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "types.h"
#include "parser.h"
#include "cleanup.h"

int	main(int argc, char **argv)
{
	t_command	*cmds;
	int			error;

	(void)argc;
	(void)argv;
	error = 0;
	cmds = parse(NULL, &error);
	printf("parse -> cmds=%p error=%d\n", (void *)cmds, error);
	free_commands(cmds);
	return (0);
}
