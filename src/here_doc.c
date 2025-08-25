/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:41:46 by carbon            #+#    #+#             */
/*   Updated: 2025/08/25 16:59:47 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_heredoc(const char *delimiter)
{
	char	*line;
	int		fd;

	fd = open("/tmp/.minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("heredoc");
		return ;
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
}

char	*append_line_to_content(char *content, char *line, size_t *len)
{
	size_t	line_len;
	char	*new_content;

	line_len = ft_strlen(line);
	new_content = malloc(*len + line_len + 2);
	if (!new_content)
		return (NULL);
	if (content)
	{
		ft_memcpy(new_content, content, *len);
		free(content);
	}
	ft_memcpy(new_content + *len, line, line_len);
	new_content[*len + line_len] = '\n';
	new_content[*len + line_len + 1] = '\0';
	*len += line_len + 1;
	return (new_content);
}

char	*read_heredoc_content(const char *delimiter)
{
	char	*line;
	char	*content;
	size_t	len;

	content = NULL;
	len = 0;
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
		{
			free(line);
			break ;
		}
		content = append_line_to_content(content, line, &len);
		free(line);
	}
	return (content);
}
