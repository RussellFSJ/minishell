/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:48 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/22 17:21:41 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "cleanup.h"
#include "env.h"
#include "libft.h"

static int	check(int condition, const char *name)
{
	if (condition)
	{
		printf("[PASS] %s\n", name);
		return (0);
	}
	printf("[FAIL] %s\n", name);
	return (1);
}

static int	test_values(void)
{
	char	*source[4];
	t_env	*env;
	int		failed;

	source[0] = "A=1";
	source[1] = "B=two=three";
	source[2] = "EMPTY=";
	source[3] = NULL;
	env = env_init(source);
	failed = 0;
	failed += check(env_get(env, "A")
			&& ft_strcmp(env_get(env, "A"), "1") == 0, "env_get existing");
	failed += check(env_get(env, "MISSING") == NULL, "env_get missing");
	failed += check(env_get(env, "B")
			&& ft_strcmp(env_get(env, "B"), "two=three") == 0,
			"env_init preserves equals");
	failed += check(env_get(env, "EMPTY")
			&& env_get(env, "EMPTY")[0] == '\0', "empty value");
	failed += check(env_set(&env, "B", "updated") == 0 && env_get(env, "B")
			&& ft_strcmp(env_get(env, "B"), "updated") == 0, "env_set update");
	failed += check(env_set(&env, "NEW", "created") == 0 && env_get(env, "NEW")
			&& ft_strcmp(env_get(env, "NEW"), "created") == 0, "env_set add");
	free_env(env);
	return (failed);
}

static int	test_unset(void)
{
	char	*source[4];
	t_env	*env;
	int		failed;

	source[0] = "HEAD=1";
	source[1] = "MIDDLE=2";
	source[2] = "TAIL=3";
	source[3] = NULL;
	env = env_init(source);
	failed = 0;
	env_unset(&env, "MIDDLE");
	failed += check(env && env->next
			&& ft_strcmp(env->next->key, "TAIL") == 0
			&& env->next->next == NULL, "env_unset middle");
	env_unset(&env, "HEAD");
	failed += check(env && ft_strcmp(env->key, "TAIL") == 0,
			"env_unset head");
	env_unset(&env, "TAIL");
	failed += check(env == NULL, "env_unset final");
	free_env(env);
	return (failed);
}

static int	test_array(void)
{
	t_env	*env;
	char	**array;
	int		failed;

	env = env_new("A", "1", 1);
	env_add_back(&env, env_new("EMPTY", "", 1));
	env_add_back(&env, env_new("LOCAL", "secret", 0));
	env_add_back(&env, env_new("NOVALUE", NULL, 1));
	array = env_to_array(env);
	failed = 0;
	failed += check(array != NULL, "env_to_array allocation");
	failed += check(array && array[0]
			&& ft_strcmp(array[0], "A=1") == 0, "array normal value");
	failed += check(array && array[1]
			&& ft_strcmp(array[1], "EMPTY=") == 0, "array empty value");
	failed += check(array && array[2] == NULL,
			"array filtering and terminator");
	free_array(array);
	free_env(env);
	return (failed);
}

int	main(void)
{
	int	failed;

	printf("=== environment tests ===\n\n");
	failed = 0;
	failed += test_values();
	failed += test_unset();
	failed += test_array();
	free_env(NULL);
	free_array(NULL);
	if (failed == 0)
	{
		printf("\nAll environment tests passed.\n");
		return (0);
	}
	printf("\n%d environment test(s) failed.\n", failed);
	return (1);
}
