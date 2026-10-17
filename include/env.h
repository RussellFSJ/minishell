/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/06 12:13:48 by ebin-ahm          #+#    #+#             */
/*   Updated: 2026/10/17 16:55:46 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# include "types.h"

t_env	*env_init(char **envp);
t_env	*env_new(char *key, char *value, int exported);
void	env_add_back(t_env **env, t_env *node);
t_env	*env_find(t_env *env, const char *key);
char	*env_get(t_env *env, const char *key);
int		env_set(t_env **env, const char *key, const char *value);
int		env_unset(t_env **env, const char *key);
char	**env_to_array(t_env *env);
void	free_env(t_env *env);

#endif
