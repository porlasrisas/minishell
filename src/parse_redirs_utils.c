/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 14:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/05 14:48:08 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	grow_redirs(t_redirection **arr, int *count)
{
	size_t			new_size;
	t_redirection	*new_array;

	new_size = (*count + 1) * sizeof(t_redirection);
	new_array = ft_realloc(*arr, *count * sizeof(t_redirection), new_size);
	if (!new_array)
		return (0);
	*arr = new_array;
	return (1);
}

char	*process_redir_target(t_shell *shell, char *file, t_redir_type type,
		int *no_expand)
{
	size_t	len;

	if (type == REDIR_HEREDOC)
	{
		*no_expand = 0;
		if (file)
		{
			len = ft_strlen(file);
			if ((file[0] == '\'' && file[len - 1] == '\'') || (file[0] == '"'
					&& file[len - 1] == '"'))
				*no_expand = 1;
		}
		return (ft_remove_quotes(file));
	}
	return (ft_process_token_quotes(shell, file));
}
