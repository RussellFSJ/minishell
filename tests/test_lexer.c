/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_lexer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:54 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/23 04:39:36 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

int	main(void)
{
	t_token	*tokens;
	int		error;

	tokens = lex("cat < infile | grep \"hello world\" >> outfile", &error);
	printf("tokens=%p error=%d\n", (void *)tokens, error);
	dump_tokens(tokens, 1);
	return (0);
}

