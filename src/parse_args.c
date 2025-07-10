/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:50:31 by carbon            #+#    #+#             */
/*   Updated: 2025/07/10 20:53:48 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_is_flag(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
		return (1);
	return (0);
}

void ft_args_with_flags(char **tokens, t_command *cmd)
{
	int		i = 0;
	int		count = 0;
	char	**flagged = NULL;

	while (tokens[i])
	{
		char *entry = NULL;

		if (ft_is_flag(tokens[i]))
		{
			if (tokens[i + 1] && !ft_is_flag(tokens[i + 1]))
			{
				char *tmp = ft_strjoin(tokens[i], " ");
				entry = ft_strjoin(tmp, tokens[i + 1]);
				free(tmp);
				i += 2;
			}
			else
				entry = ft_strdup(tokens[i++]);
		}
		else
			entry = ft_strdup(tokens[i++]);
		flagged = ft_realloc(flagged, sizeof(char *) * count, sizeof(char *) * (count + 1));
		flagged[count++] = entry;
	}
	flagged = ft_realloc(flagged, sizeof(char *) * count, sizeof(char *) * (count + 1));
	flagged[count] = NULL;
	cmd->args_flagged = flagged;
	i = 0;
	while (cmd->args_flagged[i])
	{
		printf("Args con flag: %s\n", cmd->args_flagged[i]);
		++i;
	}
}