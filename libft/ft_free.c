/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 23:05:54 by guigonza          #+#    #+#             */
/*   Updated: 2025/05/28 00:28:20 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdarg.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>


static void	ft_free_depth(void **ptr)
{
	int	i;

	i = 0;
	if (!ptr)
		return ;
	while (ptr[i])
	{
		if (*(void **)ptr[i])
		{
			ft_free_depth((void **)ptr[i]);
		}
		free(ptr[i]);
		ptr[i] = NULL;
		i++;
	}
	free(ptr);
}

void	ft_cleaner(int count, ...)
{
	va_list		args;
	t_format	**format;
	int			i;

	i = 0;
	va_start(args, count);
	while (i < count)
	{
		format = va_arg(args, t_format **);
		if (format && *format)
		{
			if ((*format)->ptr)
			{
				ft_free_depth((*format)->ptr);
				(*format)->ptr = NULL;
			}
		}
		free(*format);
		*format = NULL;
		i++;
	}
	va_end(args);
}
void	ft_error(const char	*custom_msg, int count, ...)
{
	va_list	args;

	if(custom_msg)
		ft_putstr_fd((char *)custom_msg, 2);
	if (errno)
		perror("Reason: ");
	va_start(args, count);
	ft_cleaner(count, args);
	va_end(args);
}