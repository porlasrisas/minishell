/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:30:10 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 22:30:10 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_join_flags(t_command *cmd, int *i)
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

char	**ft_realloc_flagged(char **flagged, int count)
{
	char	**new_flagged;

	new_flagged = ft_realloc(flagged, sizeof(char *) * count, sizeof(char *)
			* (count + 2));
	if (!new_flagged)
		return (flagged);
	return (new_flagged);
}

void	ft_push_flagged(char ***flagged, int *count, char *entry)
{
	if (!*flagged)
		*flagged = NULL;
	*flagged = ft_realloc_flagged(*flagged, *count);
	if (*flagged && entry)
	{
		(*flagged)[*count] = entry;
		(*count)++;
	}
}

void	ft_finalize_flagged(char ***flagged, int count)
{
	if (*flagged)
	{
		*flagged = ft_realloc_flagged(*flagged, count);
		(*flagged)[count] = NULL;
	}
}
