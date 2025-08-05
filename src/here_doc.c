/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:41:46 by carbon            #+#    #+#             */
/*   Updated: 2025/08/03 19:03:10 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_heredoc(const char *delimiter)
{
    char	*line;
    int		fd;
    
    fd = open("/tmp/.minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (fd == -1)
    {
        perror("heredoc");
        return;
    }
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
}

char *read_heredoc_content(const char *delimiter)
{
    char *line;
    char *content = NULL;
    size_t len = 0;

    while (1)
    {
        line = readline("> ");
        if (!line || ft_strncmp(line, delimiter, ft_strlen(delimiter)) == 0)
        {
            free(line);
            break;
        }
        size_t line_len = ft_strlen(line);
        char *new_content = malloc(len + line_len + 2); // +1 for '\n', +1 for '\0'
        if (content)
        {
            ft_memcpy(new_content, content, len);
            free(content);
        }
        ft_memcpy(new_content + len, line, line_len);
        new_content[len + line_len] = '\n';
        new_content[len + line_len + 1] = '\0';
        content = new_content;
        len += line_len + 1;
        free(line);
    }
    return content;
}
