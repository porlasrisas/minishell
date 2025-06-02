/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:05:54 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/02 16:09:58 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


static void	ft_deep_free(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
}

static void	ft_shallow_free(void *ptr)
{
	if (ptr)
		free(ptr);
}

void	ft_error(const char *custom_msg, int count, ...)
{
	va_list	ap;
	int		i;
	void	*ptr;
	int		type;

	if (custom_msg)
		ft_putstr_fd((char *)custom_msg, 2);
	if (errno)
		perror("Reason: ");
	va_start(ap, count);
	i = 0;
	while (i < count)
	{
		type = va_arg(ap, int);
		ptr = va_arg(ap, void *);
		if (type == 0)
			ft_shallow_free(ptr);
		else if (type == 1)
			ft_deep_free((void **)ptr);
		i++;
	}
	va_end(ap);
}