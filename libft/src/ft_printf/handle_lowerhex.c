/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_lowerhex.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/14 17:48:58 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:14 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_lowerhex(va_list *args)
{
	unsigned int	n;

	n = va_arg(*args, unsigned int);
	return (print_hex((unsigned long)n, 0));
}
