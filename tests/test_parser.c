/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:56 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/30 02:04:17 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*redir_name(t_token_type type)
{
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

static void	dump_redirs(t_redir *redirs)
{
	while (redirs)
	{
		ft_printf("  %s: %s", redir_name(redirs->type), redirs->target);
		ft_printf(" expand=%d fd=%d\n", redirs->expand_body, redirs->fd);
		redirs = redirs->next;
	}
}

static void	dump_commands(t_command *cmds)
{
	int	i;
	int	n;

	n = 0;
	while (cmds)
	{
		ft_printf("COMMAND %d\n", n++);
		i = 0;
		if (!cmds->argv)
			ft_printf("  argv: (none)\n");
		while (cmds->argv && cmds->argv[i])
		{
			ft_printf("  argv[%d]: %s\n", i, cmds->argv[i]);
			i++;
		}
		dump_redirs(cmds->redirs);
		cmds = cmds->next;
	}
}

static void	run_test(char *line)
{
	t_token		*tokens;
	t_command	*cmds;
	int			error;

	ft_printf("=== %s ===\n", line);
	error = 0;
	tokens = lex(line, &error);
	if (error == -1)
	{
		ft_printf("LEX ERROR\n\n");
		return ;
	}
	cmds = parse(tokens, &error);
	if (error == -1)
		ft_printf("PARSER ERROR\n");
	else
		dump_commands(cmds);
	free_commands(cmds);
	free_tokens(tokens);
	ft_printf("\n");
}

int	main(void)
{
	run_test("echo hello");
	run_test("ls -la /tmp");
	run_test("echo hi | cat");
	run_test("cat < infile");
	run_test("echo hi > outfile");
	run_test("echo hi >> outfile");
	run_test("cat << EOF");
	run_test("cat < in | grep x > out");
	run_test("| cat");
	run_test("cat |");
	run_test("cat >");
	return (0);
}
