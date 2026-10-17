/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:40 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 21:08:03 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	shell_init(t_shell *shell, char **envp);
static void	read_one_line(t_shell *shell);
static void	process_line(t_shell *shell, const char *line);
static void	frontend_error(t_shell *shell, int error);

int	minishell(char **envp)
{
	t_shell	shell;

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
	{
		add_history(line);
		process_line(shell, line);
	}
	free(line);
}

static void	process_line(t_shell *shell, const char *line)
{
	t_token		*tokens;
	t_command	*cmds;
	int			error;

	tokens = lex(line, &error);
	if (error)
		return (frontend_error(shell, error));
	cmds = parse(tokens, &error);
	free_tokens(tokens);
	if (error)
		return (frontend_error(shell, error));
	if (expand(shell, cmds) == -1)
		frontend_error(shell, -1);
	else
		execute(shell, cmds);
	free_commands(cmds);
}

static void	frontend_error(t_shell *shell, int error)
{
	if (error == -2)
	{
		ft_putendl_fd("minishell: syntax error", STDERR_FILENO);
		shell->last_status = 2;
	}
	else
	{
		ft_putendl_fd("minishell: memory allocation failed", STDERR_FILENO);
		shell->last_status = 1;
	}
}
