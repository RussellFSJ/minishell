/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:01 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/23 08:23:33 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

typedef void					(*t_signal_handler)(int);

extern volatile sig_atomic_t	g_signal;

void	signals_setup_prompt(void);
void	signals_setup_child(void);
void	signals_setup_wait(void);
void	signals_setup_heredoc(void);
void	signals_set_handler(int signum, void (*handler)(int));

#endif
