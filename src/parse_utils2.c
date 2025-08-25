/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 16:58:56 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_remove_quotes(char *token)
{
	char	*result;
	int		len;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (ft_strdup(token));
	if ((token[0] == '\"' && token[len - 1] == '\"') || (token[0] == '\''
			&& token[len - 1] == '\''))
	{
		result = ft_substr(token, 1, len - 2);
		return (result);
	}
	return (ft_strdup(token));
}
