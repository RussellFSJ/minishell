/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdouble.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 15:24:06 by rfoo              #+#    #+#             */
/*   Updated: 2026/06/17 15:21:41 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdouble(const char *str)
{
	int	i;
	int	has_digit;

	i = 0;
	has_digit = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (ft_isdigit(str[i]))
	{
		has_digit = 1;
		i++;
	}
	if (str[i] == '.')
		i++;
	while (ft_isdigit(str[i]))
	{
		has_digit = 1;
		i++;
	}
	return (str[i] == '\0' && has_digit);
}
