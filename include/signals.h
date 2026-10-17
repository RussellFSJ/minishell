/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:01 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/14 22:33:41 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include <signal.h>

typedef void					(*t_signal_handler)(int);

extern volatile sig_atomic_t	g_signal;

void	signals_setup_prompt(void);
void	signals_setup_child(void);
void	signals_setup_wait(void);
void	signals_setup_heredoc(void);

void	signals_set_handler(int signum, void (*handler)(int));

#endif
