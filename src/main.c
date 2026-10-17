/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/14 21:26:47 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shell_init(t_shell *shell, char **envp);
static void	read_one_line(t_shell *shell);

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

static void	shell_init(t_shell *shell, char **envp)
{
	shell->env = env_init(envp);
	shell->last_status = 0;
	shell->should_exit = 0;
}

static void	read_one_line(t_shell *shell)
{
	char	*line;

	signals_setup_prompt();
	line = readline("minishell$ ");
	if (g_signal == SIGINT)
		shell->last_status = 130;
	g_signal = 0;
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
