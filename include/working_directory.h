/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   working_directory.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/23 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/11/20 15:26:17 by rfoo             ###   ########.fr       */
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
