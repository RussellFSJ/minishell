/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_upperhex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 17:49:09 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:21 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_upperhex(va_list *args)
{
	unsigned int	n;

	n = va_arg(*args, unsigned int);
	return (print_hex((unsigned long)n, 1));
}
