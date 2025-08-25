/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 19:50:31 by carbon            #+#    #+#             */
/*   Updated: 2025/08/24 22:44:20 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_flag(char *token)
{
	if (token[0] == '-' && token[1] != '\0')
		return (1);
	return (0);
}

void	ft_args_with_flags(t_command *cmd)
{
	int		i;
	int		count;
	char	**flagged;
	char	*entry;

	if (!cmd || !cmd->args)
		return ;
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
		ft_push_flagged(&flagged, &count, entry);
	}
	ft_finalize_flagged(&flagged, count);
	cmd->args_flagged = flagged;
}
