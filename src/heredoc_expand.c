/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expand.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 14:25:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/05 14:48:08 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*itoa_snapshot(int n)
{
	return (ft_itoa(n));
}

static char	*append_dollar_expansion(t_shell *shell, char *line, int *i,
		int use_snapshot)
{
	if (line[*i + 1] == '?')
	{
		*i += 2;
		if (use_snapshot)
			return (itoa_snapshot(shell->hd_snapshot_status));
		return (itoa_snapshot(shell->exit_status));
	}
	return (ft_expand_variable_in_string(shell, line, i));
}

char	*expand_heredoc_line(t_shell *shell, char *line)
{
	char	*result;
	int		i;
	int		use_snapshot;

	result = ft_strdup("");
	i = 0;
	use_snapshot = shell->hd_snapshot_set;
	while (line[i])
	{
		if (line[i] == '$')
			result = ft_strjoin_free(result, append_dollar_expansion(shell,
						line, &i, use_snapshot));
		else
		{
			result = ft_append_char(result, line[i]);
			i++;
		}
	}
	return (result);
}
