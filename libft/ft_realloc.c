/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 13:28:11 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/03 13:36:09 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void *new;
	size_t size;

	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new = malloc(new_size);
	if (!new)
		return (NULL);
	if (old_size < new_size)
		size = old_size;
	else
		size = new_size;
	ft_memcpy(new, ptr, size);
	free(ptr);
	return (new);
}