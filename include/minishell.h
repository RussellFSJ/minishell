/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 19:59:44 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/15 04:53:31 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

# include "types.h"
# include "signals.h"
# include "builtins.h"
# include "cleanup.h"
# include "working_directory.h"
# include "environment.h"
# include "executor.h"
# include "expander.h"
# include "redirections.h"
# include "lexer.h"
# include "libft.h"
# include "parser.h"
# include "shell_io.h"

int	minishell(char **envp);

#endif
