/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:54 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:59:08 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "types.h"
#include "lexer.h"
#include "cleanup.h"

int	main(int argc, char **argv)
{
	t_token	*tokens;
	int		error;

	(void)argc;
	(void)argv;
	error = 0;
	tokens = lex("echo hello", &error);
	printf("lex -> tokens=%p error=%d\n", (void *)tokens, error);
	dump_tokens(tokens, 1);
	free_tokens(tokens);
	return (0);
}
