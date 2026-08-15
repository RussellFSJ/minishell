/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dictionary.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rfoo <rfoo@student.42singapore.sg>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 14:34:24 by rfoo              #+#    #+#             */
/*   Updated: 2026/03/02 14:01:07 by rfoo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	dict_set(t_dict *dict, char key, t_handler handler);
static void	add_entries(t_dict *dict);

t_dict	*dict_init(void)
{
	const char	*keys;
	t_dict		*printf_dict;

	keys = "cspdiuxX%";
	printf_dict = malloc(sizeof(t_dict));
	if (!printf_dict)
		return (NULL);
	printf_dict->size = ft_strlen(keys);
	printf_dict->entries = ft_calloc(ft_strlen(keys), sizeof(t_entry));
	if (!printf_dict->entries)
	{
		free(printf_dict);
		return (NULL);
	}
	add_entries(printf_dict);
	return (printf_dict);
}

static void	dict_set(t_dict *dict, char key, t_handler handler)
{
	int		index;
	t_entry	entry;

	index = 0;
	entry.key = key;
	entry.handler = handler;
	while (index < dict->size)
	{
		if (dict->entries[index].key == key)
		{
			dict->entries[index].handler = handler;
			return ;
		}
		if (!dict->entries[index].key)
		{
			dict->entries[index] = entry;
			return ;
		}
		index++;
	}
}

static void	add_entries(t_dict *dict)
{
	dict_set(dict, 'c', handle_char);
	dict_set(dict, 's', handle_str);
	dict_set(dict, 'p', handle_ptr);
	dict_set(dict, 'd', handle_int);
	dict_set(dict, 'i', handle_int);
	dict_set(dict, 'u', handle_uint);
	dict_set(dict, 'x', handle_lowerhex);
	dict_set(dict, 'X', handle_upperhex);
	dict_set(dict, '%', handle_percent);
}

t_handler	dict_get(t_dict *dict, char key)
{
	int	index;

	index = 0;
	while (index < dict->size)
	{
		if (dict->entries[index].key == key)
			return (dict->entries[index].handler);
		index++;
	}
	return (NULL);
}

void	dict_free(t_dict *dict)
{
	if (!dict)
		return ;
	if (dict->entries)
		free(dict->entries);
	free(dict);
}
