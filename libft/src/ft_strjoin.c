/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 04:35:27 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:00:50 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	strlen1;
	size_t	strlen2;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return (ft_strdup(s2));
	if (!s2)
		return (ft_strdup(s1));
	strlen1 = ft_strlen(s1);
	strlen2 = ft_strlen(s2);
	str = malloc(strlen1 + strlen2 + 1);
	if (!str)
		return (NULL);
	ft_memcpy(str, s1, strlen1);
	ft_memcpy(str + strlen1, s2, strlen2);
	str[strlen1 + strlen2] = '\0';
	return (str);
}
