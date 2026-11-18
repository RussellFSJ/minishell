/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setup_prompt.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:36 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:58:33 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal = 0;

static void	prompt_interrupt(int signum);

void	signals_setup_prompt(void)
{
	rl_catch_signals = 0;
	signals_set_handler(SIGINT, prompt_interrupt);
	signals_set_handler(SIGQUIT, SIG_IGN);
}

static void	prompt_interrupt(int signum)
{
	int	saved_errno;

	saved_errno = errno;
	g_signal = signum;
	if (write_all(STDOUT_FILENO, "\n", 1) == 0)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	errno = saved_errno;
}
