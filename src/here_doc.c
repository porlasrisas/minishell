/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 18:41:46 by carbon            #+#    #+#             */
/*   Updated: 2025/09/05 14:23:18 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* moved to heredoc_utils.c: hd_is_exact_delim */

/* moved to heredoc_utils.c: hd_append_line */

void	handle_heredoc(const char *delimiter)
{
	char	*line;
	int		fd;

	fd = open("/tmp/.minishell_heredoc", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return ;
	while (1)
	{
		line = readline("> ");
		if (!line || hd_is_exact_delim(line, delimiter))
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
}

/* expand_line movido a heredoc_expand.c como expand_heredoc_line */

static int	should_stop(char *line, const char *delimiter)
{
	if (!line || hd_is_exact_delim(line, delimiter))
		return (1);
	return (0);
}

char	*read_heredoc_content(t_shell *shell, const char *delimiter,
		int no_expand)
{
	char	*line;
	char	*content;
	size_t	len;
	char	*tmp;

	content = NULL;
	len = 0;
	while (1)
	{
		line = readline("> ");
		if (should_stop(line, delimiter))
			break ;
		if (!no_expand)
		{
			tmp = expand_heredoc_line(shell, line);
			free(line);
			line = tmp;
		}
		if (!hd_append_line(&content, &len, line))
			break ;
		free(line);
	}
	free(line);
	return (content);
}

/* moved to heredoc_utils.c: hd_init_snapshot_if_needed */

void	precollect_heredocs(t_shell *shell)
{
	int	i;
	int	j;

	shell->hd_snapshot_set = 0;
	i = 0;
	while (i < shell->command_count)
	{
		j = 0;
		while (j < shell->commands[i]->redir_count)
		{
			if (shell->commands[i]->redirs[j].type == REDIR_HEREDOC)
			{
				hd_init_snapshot_if_needed(shell);
				return ;
			}
			j++;
		}
		i++;
	}
}

void	collect_heredocs_for_cmd(t_shell *shell, t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC
			&& cmd->redirs[i].heredoc_content == NULL)
			cmd->redirs[i].heredoc_content = read_heredoc_content(shell,
					cmd->redirs[i].file, cmd->redirs[i].hd_no_expand);
		i++;
	}
}
