/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:50:19 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "types.h"
#include "env.h"

static void	shell_init(t_shell *shell, char **envp)
{
	shell->env = env_init(envp);
	shell->last_status = 0;
	shell->should_exit = 0;
}

static void	read_one_line(t_shell *shell)
{
	char	*line;

	line = readline("minishell$ ");
	if (line == NULL)
	{
		write(STDERR_FILENO, "exit\n", 5);
		shell->should_exit = 1;
		return ;
	}
	if (line[0] != '\0')
		add_history(line);
	free(line);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	shell_init(&shell, envp);
	while (shell.should_exit == 0)
		read_one_line(&shell);
	free_env(shell.env);
	rl_clear_history();
	return (shell.last_status);
}
