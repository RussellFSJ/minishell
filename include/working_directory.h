/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working_directory.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: russ1337 <russ1337@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/23 08:13:40 by russ1337         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WORKING_DIRECTORY_H
# define WORKING_DIRECTORY_H

int		cwd_matches(const char *path);
int		cwd_initialize(t_shell *shell);
char	*cwd_current(t_shell *shell);
char	*cwd_target(t_shell *shell, const char *target);
char	*cwd_normalize(const char *path);

#endif
