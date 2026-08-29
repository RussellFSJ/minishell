/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:46 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:43:07 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	test_dispatch(t_shell *shell)
{
	t_command	cmd;
	char		*args[3];

	args[0] = "echo";
	args[1] = "dispatch works";
	args[2] = NULL;
	cmd.argv = args;
	cmd.redirs = NULL;
	cmd.next = NULL;
	printf("is_builtin(echo) = %d\n", is_builtin("echo"));
	printf("is_builtin(ls) = %d\n", is_builtin("ls"));
	printf("dispatch echo:\n");
	fflush(stdout);
	printf("status = %d\n", builtin_dispatch(shell, &cmd));
}

static void	test_echo_pwd(t_shell *shell)
{
	char	*echo_args[4];
	char	*pwd_args[2];

	echo_args[0] = "echo";
	echo_args[1] = "-nn";
	echo_args[2] = "hello";
	echo_args[3] = NULL;
	pwd_args[0] = "pwd";
	pwd_args[1] = NULL;
	printf("\necho -nn hello: [");
	fflush(stdout);
	builtin_echo(shell, echo_args);
	printf("]\npwd:\n");
	fflush(stdout);
	printf("pwd status = %d\n", builtin_pwd(shell, pwd_args));
}

static void	test_env_unset(t_shell *shell)
{
	char	*env_args[2];
	char	*unset_args[3];
	char	*value;

	env_args[0] = "env";
	env_args[1] = NULL;
	unset_args[0] = "unset";
	unset_args[1] = "REMOVE_ME";
	unset_args[2] = NULL;
	printf("\nenv before unset:\n");
	fflush(stdout);
	builtin_env(shell, env_args);
	value = env_get(shell->env, "REMOVE_ME");
	printf("REMOVE_ME before = [%s]\n", value);
	printf("unset status = %d\n", builtin_unset(shell, unset_args));
	value = env_get(shell->env, "REMOVE_ME");
	if (!value)
		printf("REMOVE_ME after = [NULL]\n");
	else
		printf("REMOVE_ME after = [%s]\n", value);
}

int	main(void)
{
	char		*envp[4];
	t_shell	shell;

	envp[0] = "USER=ebin-ahm";
	envp[1] = "HOME=/home/ebin-ahm";
	envp[2] = "REMOVE_ME=test";
	envp[3] = NULL;
	shell.env = env_init(envp);
	shell.last_status = 0;
	shell.should_exit = 0;
	test_dispatch(&shell);
	test_echo_pwd(&shell);
	test_env_unset(&shell);
	free_env(shell.env);
	return (0);
}
