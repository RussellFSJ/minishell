/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_expander.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/30 05:05:21 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 05:13:19 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "minishell.h"

static void	print_redirs(t_redir *redir)
{
	while (redir)
	{
		printf("  REDIR type=%d target=[%s] expand_body=%d\n",
			redir->type, redir->target, redir->expand_body);
		redir = redir->next;
	}
}

static void	print_commands(t_command *cmds)
{
	int	i;
	int	n;

	n = 0;
	while (cmds)
	{
		printf("COMMAND %d\n", n++);
		i = 0;
		while (cmds->argv && cmds->argv[i])
		{
			printf("  argv[%d]: [%s]\n", i, cmds->argv[i]);
			i++;
		}
		print_redirs(cmds->redirs);
		cmds = cmds->next;
	}
}

static void	run_test(t_shell *shell, const char *line)
{
	t_token		*tokens;
	t_command	*cmds;
	int			error;

	printf("\n=== %s ===\n", line);
	error = 0;
	tokens = lex(line, &error);
	if (error)
		return ;
	cmds = parse(tokens, &error);
	free_tokens(tokens);
	if (error || !cmds)
		return ;
	if (expand(shell, cmds) == -1)
		printf("EXPANSION ERROR\n");
	else
		print_commands(cmds);
	free_commands(cmds);
}

int	main(void)
{
	char		*envp[4];
	t_shell	shell;

	envp[0] = "USER=ebin-ahm";
	envp[1] = "HOME=/home/ebin-ahm";
	envp[2] = "EMPTY=";
	envp[3] = NULL;
	shell.env = env_init(envp);
	shell.last_status = 127;
	shell.should_exit = 0;
	run_test(&shell, "echo \"$USER\" '$USER' $?");
	run_test(&shell, "echo abc\"$USER\" \"$HOME/test\" | cat");
	run_test(&shell, "cat < \"$HOME/input\" > '$USER.out'");
	run_test(&shell, "cat << \"EOF\"");
	free_env(shell.env);
	return (0);
}
