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
	shell_release(&shell);
	return (shell.last_status);
}

static void	shell_init(t_shell *shell, char **envp)
{
	ft_bzero(shell, sizeof(*shell));
	shell->env = env_init(envp);
	if ((!shell->env && envp && envp[0]) || cwd_initialize(shell))
	{
		ft_putendl_fd("minishell: initialization failed", 2);
		shell->last_status = 1;
		shell->should_exit = 1;
	}
}

static void	read_one_line(t_shell *shell)
{
	signals_setup_prompt();
	shell->line = readline("minishell$ ");
	if (g_signal == SIGINT)
		shell->last_status = 130;
	g_signal = 0;
	if (shell->line == NULL)
	{
		if (write_all(STDERR_FILENO, "exit\n", 5))
			shell->last_status = 1;
		shell->should_exit = 1;
		return ;
	}
	if (shell->line[0] != '\0')
	{
		add_history(shell->line);
		process_line(shell, shell->line);
	}
	free(shell->line);
	shell->line = NULL;
}

static void	process_line(t_shell *shell, const char *line)
{
	t_token		*tokens;
	t_command	*cmds;
	int			error;

	tokens = lex(line, &error);
	if (error)
		return (frontend_error(shell, error));
	cmds = parser_parse(tokens, &error);
	free_tokens(tokens);
	if (error)
		return (frontend_error(shell, error));
	shell->commands = cmds;
	if (expand(shell, cmds) == -1)
		frontend_error(shell, -1);
	else
		executor_run(shell, cmds);
	free_commands(cmds);
	shell->commands = NULL;
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
