/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:14:16 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 15:34:57 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_pwd(t_shell *shell)
{
	char	*cwd;
	int		status;

	cwd = cwd_current(shell);
	if (!cwd)
	{
		ft_putendl_fd("minishell: pwd: error", 2);
		return (1);
	}
	status = 0;
	if (builtin_putline(cwd))
		status = builtin_output_error("pwd");
	free(cwd);
	return (status);
}
