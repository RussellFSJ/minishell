/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_all.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/22 00:00:00 by rfoo              #+#    #+#             */
/*   Updated: 2026/10/22 00:00:00 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	write_all(int fd, const char *text, size_t length)
{
	ssize_t	written;

	while (length)
	{
		written = write(fd, text, length);
		if (written < 0 && errno == EINTR)
			continue ;
		if (written == 0)
			errno = EIO;
		if (written <= 0)
			return (1);
		text += written;
		length -= written;
	}
	return (0);
}
