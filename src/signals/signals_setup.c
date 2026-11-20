/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:36 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals_set_handler(int signum, void (*handler)(int))
{
	if (signal(signum, handler) == SIG_ERR)
		perror("minishell: signal");
}

void	signals_setup_child(void)
{
	signals_set_handler(SIGINT, SIG_DFL);
	signals_set_handler(SIGQUIT, SIG_DFL);
}

void	signals_setup_wait(void)
{
	signals_set_handler(SIGINT, SIG_IGN);
	signals_set_handler(SIGQUIT, SIG_IGN);
}
