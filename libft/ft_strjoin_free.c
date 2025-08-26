/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 17:16:52 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/26 17:31:37 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin_free(char *s1, const char *s2)
{
	size_t	i;
	size_t	j;
	char	*result;

	i = 0;
	j = 0;
	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (ft_strdup(s2));
	if (s2 == NULL)
	{
		return (s1);
	}
	result = malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!result)
	{
		free(s1);
		return (NULL);
	}
	while (s1 && s1[i] != '\0')
	{
		result[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
	{
		result[i] = s2[j];
		i++;
		j++;
	}
	result[i] = '\0';
	free (s1);
	return (result);
}
