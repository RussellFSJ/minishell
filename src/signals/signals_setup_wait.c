/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals_setup_wait.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebin-ahm <ebin-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:15:36 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/08/06 14:58:33 by ebin-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signals_setup_wait(void)
{
	signals_set_handler(SIGINT, SIG_IGN);
	signals_set_handler(SIGQUIT, SIG_IGN);
}
