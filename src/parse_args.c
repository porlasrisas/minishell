/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:50:31 by carbon            #+#    #+#             */
/*   Updated: 2025/07/15 13:23:20 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_is_flag(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
		return (1);
	return (0);
}


void ft_args_with_flags(t_command *cmd)
{
	int		i = 0;
	int		count = 0;
	char 	*tmp;
	char	**flagged = NULL;
	char 	*entry;
	
	while (cmd->args[i])
	{
		entry = NULL;
		if (ft_is_flag(cmd->args[i]))
		{
			tmp = ft_strjoin(cmd->args[i - 1], " ");
			entry = ft_strjoin_free(tmp, cmd->args[i]);
			i++;
			while (cmd->args[i] && ft_is_flag(cmd->args[i]))
			{
				tmp = ft_strjoin_free(entry, " ");
				entry = ft_strjoin(tmp, cmd->args[i]);
				++i;
			}
		}
		else
			++i;
		flagged = ft_realloc(flagged, sizeof(char *) * count, sizeof(char *) * (count + 1));
		if (entry)
			flagged[count++] = entry;
	}
	flagged = ft_realloc(flagged, sizeof(char *) * count, sizeof(char *) * (count + 1));
	flagged[count] = NULL;
	cmd->args_flagged = flagged;
}
