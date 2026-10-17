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

# include "builtins.h"
# include "cleanup.h"
# include "env.h"
# include "executor.h"
# include "expander.h"
# include "lexer.h"
# include "libft.h"
# include "parser.h"
# include "signals.h"
# include "types.h"

# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>

int	minishell(char **envp);

#endif
