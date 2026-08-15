/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 19:15:17 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:00:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*p1;
	const unsigned char	*p2;

	i = 0;
	p1 = (unsigned char *)dest;
	p2 = (const unsigned char *)src;
	if (!dest && !src)
		return (NULL);
	while (i < n)
	{
		if (p1 < p2)
			p1[i] = p2[i];
		else
			p1[n - 1 - i] = p2[n - 1 - i];
		i++;
	}
	return (dest);
}
