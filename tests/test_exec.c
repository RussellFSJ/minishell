/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_exec.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:51 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:59:32 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>

static int run(t_shell *shell, char **args)
{
	t_command cmd = {args, NULL, NULL};
	int result;

	result = execute(shell, &cmd);
	assert(result == shell->last_status);
	return (result);
}

static void test_commands(t_shell *shell)
{
	char *ok[] = {"true", NULL};
	char *status[] = {"/bin/sh", "-c", "exit 42", NULL};
	char *missing[] = {"minishell_nonexistent_command", NULL};
	char *directory[] = {"/", NULL};
	char *signal_cmd[] = {"/bin/sh", "-c", "kill -TERM $$", NULL};
	char *environment[] = {"/bin/sh", "-c",
		"test \"$EXEC_TEST\" = inherited", NULL};

	assert(run(shell, ok) == 0);
	assert(run(shell, status) == 42);
	assert(execute(shell, NULL) == 42);
	assert(run(shell, missing) == 127);
	assert(run(shell, directory) == 126);
	assert(run(shell, signal_cmd) == 143);
	assert(run(shell, environment) == 0);
}

static void test_pipelines(t_shell *shell)
{
	char *producer[] = {"/bin/sh", "-c",
		"i=0; while [ $i -lt 20000 ]; do echo payload; i=$((i+1)); done", NULL};
	char *copy[] = {"cat", NULL};
	char *consumer[] = {"/bin/sh", "-c",
		"test \"$(wc -l)\" -eq 20000", NULL};
	char *slow[] = {"/bin/sh", "-c", "sleep 0.1; exit 7", NULL};
	char *last[] = {"/bin/sh", "-c", "exit 23", NULL};
	t_command third = {consumer, NULL, NULL};
	t_command second = {copy, NULL, &third};
	t_command first = {producer, NULL, &second};

	assert(execute(shell, &first) == 0);
	first.argv = slow;
	second.argv = last;
	second.next = NULL;
	assert(execute(shell, &first) == 23);
	errno = 0;
	assert(waitpid(-1, NULL, WNOHANG) == -1 && errno == ECHILD);
}

static void test_builtins(t_shell *shell)
{
	char *unset[] = {"unset", "EXEC_TEST", NULL};
	char *ok[] = {"true", NULL};
	char *echo[] = {"echo", "hello", NULL};
	char *read_echo[] = {"/bin/sh", "-c", "read line; test \"$line\" = hello", NULL};
	t_command second = {ok, NULL, NULL};
	t_command first = {unset, NULL, &second};

	assert(execute(shell, &first) == 0);
	assert(env_get(shell->env, "EXEC_TEST") != NULL);
	assert(run(shell, unset) == 0);
	assert(env_get(shell->env, "EXEC_TEST") == NULL);
	first.argv = echo;
	second.argv = read_echo;
	assert(execute(shell, &first) == 0);
}

static void test_paths(t_shell *shell)
{
	char name[] = "./.executor-test-XXXXXX";
	char *args[] = {name, NULL};
	char *resolved;
	int fd;

	fd = mkstemp(name);
	assert(fd >= 0);
	assert(write(fd, "#!/bin/sh\nexit 19\n", 18) == 18);
	close(fd);
	assert(run(shell, args) == 126);
	assert(chmod(name, 0700) == 0);
	assert(env_set(&shell->env, "PATH", "/missing:") == 0);
	args[0] = name + 2;
	resolved = path_resolve(shell->env, args[0]);
	assert(resolved != NULL);
	free(resolved);
	assert(run(shell, args) == 19);
	assert(env_set(&shell->env, "PATH", "") == 0);
	assert(run(shell, args) == 19);
	assert(env_unset(&shell->env, "PATH") == 0);
	assert(run(shell, args) == 19);
	assert(unlink(name) == 0);
}

int main(void)
{
	char *envp[] = {"PATH=/bin:/usr/bin", "EXEC_TEST=inherited", NULL};
	t_shell shell = {NULL, 0, 0};

	shell.env = env_init(envp);
	assert(shell.env != NULL);
	test_commands(&shell);
	test_pipelines(&shell);
	test_builtins(&shell);
	test_paths(&shell);
	free_env(shell.env);
	puts("executor tests passed");
	return (0);
}
