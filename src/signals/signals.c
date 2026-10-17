/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
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
static void	set_handler(int signum, void (*handler)(int));

void	signals_setup_prompt(void)
{
	rl_catch_signals = 0;
	set_handler(SIGINT, prompt_interrupt);
	set_handler(SIGQUIT, SIG_IGN);
}

void	signals_setup_child(void)
{
	set_handler(SIGINT, SIG_DFL);
	set_handler(SIGQUIT, SIG_DFL);
}

void	signals_setup_wait(void)
{
	set_handler(SIGINT, SIG_IGN);
	set_handler(SIGQUIT, SIG_IGN);
}

static void	prompt_interrupt(int signum)
{
	g_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	set_handler(int signum, void (*handler)(int))
{
	if (signal(signum, handler) == SIG_ERR)
		perror("minishell: signal");
}
