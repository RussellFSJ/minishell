/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setup_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:34 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:58:45 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals_setup_heredoc(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_IGN);
}
