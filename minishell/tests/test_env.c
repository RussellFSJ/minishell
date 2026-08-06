/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:48 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:58:59 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "types.h"
#include "env.h"

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;

	(void)argc;
	(void)argv;
	env = env_init(envp);
	printf("env_init  -> %p\n", (void *)env);
	printf("env_get   -> %p\n", (void *)env_get(env, "PATH"));
	printf("env_array -> %p\n", (void *)env_to_array(env));
	free_env(env);
	return (0);
}
