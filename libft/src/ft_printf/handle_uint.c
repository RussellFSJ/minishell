/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_uint.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 15:19:30 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:19 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_uint(unsigned long n);

int	handle_uint(va_list *args)
{
	unsigned int	n;

	n = va_arg(*args, unsigned int);
	return (print_uint((unsigned long)n));
}

static int	print_uint(unsigned long nbr)
{
	int	bytes;

	bytes = 0;
	if (nbr >= 10)
		bytes += print_uint(nbr / 10);
	bytes += write(1, &(char){(nbr % 10) + '0'}, 1);
	return (bytes);
}
