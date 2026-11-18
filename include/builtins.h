/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:26 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:55:49 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

int	builtin_dispatch(t_shell *shell, t_command *cmd);
int	builtin_is_builtin(const char *name);
int	builtin_echo(char **argv);
int	builtin_pwd(t_shell *shell);
int	builtin_cd(t_shell *shell, char **argv);
int	builtin_cd_move(t_shell *shell, const char *target, char *old_dir);
int	builtin_export(t_shell *shell, char **argv);
int	builtin_export_print(t_env *env);
int	builtin_unset(t_shell *shell, char **argv);
int	builtin_env(t_shell *shell);
int	builtin_exit(t_shell *shell, char **argv);
int	builtin_write(const char *text);
int	builtin_putline(const char *text);
int	builtin_output_error(const char *name);

#endif
