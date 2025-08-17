/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:50:31 by carbon            #+#    #+#             */
/*   Updated: 2025/07/21 21:12:54 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_is_flag(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
		return (1);
	return (0);
}


static char	*ft_join_flags(t_command *cmd, int *i)
{
	char	*tmp;
	char	*entry;

	tmp = ft_strjoin(cmd->args[*i - 1], " ");
	entry = ft_strjoin_free(tmp, cmd->args[*i]);
	(*i)++;
	while (cmd->args[*i] && ft_is_flag(cmd->args[*i]))
	{
		tmp = ft_strjoin_free(entry, " ");
		entry = ft_strjoin(tmp, cmd->args[*i]);
		(*i)++;
	}
	return (entry);
}

static char	**ft_realloc_flagged(char **flagged, int count)
{
	char	**new_flagged;

	new_flagged = ft_realloc(flagged, sizeof(char *) * count, 
		sizeof(char *) * (count + 2));
	if (!new_flagged)
	{
		printf("ERROR: ft_realloc falló en ft_realloc_flagged\n");
		return (flagged);
	}
	return (new_flagged);
}

void ft_args_with_flags(t_command *cmd)
{
	int		i;
	int		count;
	char	**flagged;
	char	*entry;

	if (!cmd || !cmd->args)
		return;
	i = 0;
	count = 0;
	flagged = NULL;
	while (cmd->args[i])
	{
		entry = NULL;
		if (ft_is_flag(cmd->args[i]))
			entry = ft_join_flags(cmd, &i);
		else
		{
			entry = ft_strdup(cmd->args[i]);
			i++;
		}
		flagged = ft_realloc_flagged(flagged, count);
		if (flagged && entry)
			flagged[count++] = entry;
	}
	if (flagged)
	{
		flagged = ft_realloc_flagged(flagged, count);
		flagged[count] = NULL;
	}
	cmd->args_flagged = flagged;
}
