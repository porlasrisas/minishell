/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 14:38:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/05 14:23:18 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	hd_is_exact_delim(const char *line, const char *delim)
{
	size_t	llen;
	size_t	dlen;

	if (!line || !delim)
		return (0);
	llen = ft_strlen(line);
	dlen = ft_strlen(delim);
	if (llen != dlen)
		return (0);
	return (ft_strncmp(line, delim, dlen) == 0);
}

int	hd_append_line(char **content, size_t *len, char *line)
{
	char	*new_content;
	size_t	line_len;

	line_len = ft_strlen(line);
	new_content = malloc(*len + line_len + 2);
	if (!new_content)
		return (0);
	if (*content)
	{
		ft_memcpy(new_content, *content, *len);
		free(*content);
	}
	ft_memcpy(new_content + *len, line, line_len);
	new_content[*len + line_len] = '\n';
	new_content[*len + line_len + 1] = '\0';
	*content = new_content;
	*len += line_len + 1;
	return (1);
}

void	hd_init_snapshot_if_needed(t_shell *shell)
{
	shell->hd_snapshot_set = 1;
	shell->hd_snapshot_status = shell->exit_status;
}
