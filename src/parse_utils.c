/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:11:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/21 20:09:25 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_count_pipes(t_shell *shell)
{
	int	count;

	shell->i = 0;
	count = 1;
	while (shell->tokens[shell->i])
	{
		if (ft_get_redir_type(shell->tokens[shell->i]) == PIPE)
			count++;
		shell->i++;
	}
	return (count);
}

char	**ft_copy_token_segment(t_shell *shell, int start, int end)
{
	int		len;
	char	**segment;

	len = end - start;
	segment = malloc(sizeof(char *) * (len + 1));
	if (!segment)
		return (NULL);
	shell->j = 0;
	while (shell->j < len)
	{
		segment[shell->j] = ft_strdup(shell->tokens[start + shell->j]);
		shell->j++;
	}
	segment[shell->j] = NULL;
	return (segment);
}

int	ft_validate_pipe_syntax(t_shell *shell)
{
	if (!shell->tokens[0])
		return (0);
	if (ft_get_redir_type(shell->tokens[0]) == PIPE)
		return (0);
	shell->i = 0;
	while (shell->tokens[shell->i])
	{
		if (ft_get_redir_type(shell->tokens[shell->i]) == PIPE)
		{
			if (!shell->tokens[shell->i + 1])
				return (0);
			if (ft_get_redir_type(shell->tokens[shell->i + 1]) == PIPE)
				return (0);
		}
		shell->i++;
	}
	return (1);
}
