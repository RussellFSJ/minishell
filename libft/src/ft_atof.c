/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/13 19:53:34 by rfoo              #+#    #+#             */
/*   Updated: 2026/06/17 15:22:07 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(const char *nptr)
{
	double	num;
	double	fraction;
	int		divisor;

	num = (double)ft_atoi(nptr);
	fraction = 0.0;
	divisor = 1;
	while (*nptr && *nptr != '.')
		nptr++;
	if (*nptr == '.')
	{
		nptr++;
		while (ft_isdigit(*nptr))
		{
			fraction = fraction * 10.0 + (*nptr - '0');
			divisor *= 10;
			nptr++;
		}
		if (num >= 0)
			num += fraction / divisor;
		else
			num -= fraction / divisor;
	}
	return (num);
}
