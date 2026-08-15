/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:08:17 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:17 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_ptr(va_list *args)
{
	void			*ptr;
	unsigned long	address;

	ptr = va_arg(*args, void *);
	if (ptr == NULL)
		return (write(1, "(nil)", 5));
	address = (unsigned long)ptr;
	return (write(1, "0x", 2) + print_hex(address, 0));
}
