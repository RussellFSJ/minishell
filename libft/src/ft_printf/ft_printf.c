/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 11:58:48 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:08 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	use_handler(char s, t_dict *dict, va_list *args);
static int	handle_trailing_percent(t_dict *dict, va_list *args, int bytes);

int	ft_printf(const char *s, ...)
{
	va_list		args;
	int			i;
	int			bytes;
	t_dict		*dict;

	va_start(args, s);
	i = 0;
	bytes = 0;
	dict = dict_init();
	while (s[i])
	{
		if (s[i] == '%')
		{
			if (!s[i + 1])
				return (handle_trailing_percent(dict, &args, bytes));
			i++;
			bytes += use_handler(s[i], dict, &args);
		}
		else
			bytes += write(1, &s[i], 1);
		i++;
	}
	va_end(args);
	dict_free(dict);
	return (bytes);
}

static int	use_handler(char s, t_dict *dict, va_list *args)
{
	t_handler	handler;

	handler = dict_get(dict, s);
	if (handler)
		return (handler(args));
	return (write(1, "%", 1) + write(1, &s, 1));
}

static int	handle_trailing_percent(t_dict *dict, va_list *args, int bytes)
{
	va_end(*args);
	dict_free(dict);
	return (bytes + write(1, "%", 1));
}
