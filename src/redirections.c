/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/09/05 13:00:41 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redir_in(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

static void	handle_redir_out(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_redir_append(t_redirection *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		exit(1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

static void	handle_redir_heredoc(t_shell *shell, t_redirection *redir)
{
	int	pipefd[2];

	if (!redir->heredoc_content)
		redir->heredoc_content = read_heredoc_content(shell, redir->file,
				redir->hd_no_expand);
	if (redir->heredoc_content)
	{
		if (pipe(pipefd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		write(pipefd[1], redir->heredoc_content,
			ft_strlen(redir->heredoc_content));
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		free(redir->heredoc_content);
		redir->heredoc_content = NULL;
	}
}

void	apply_redirs(t_shell *shell, t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_IN)
			handle_redir_in(&cmd->redirs[i]);
		else if (cmd->redirs[i].type == REDIR_OUT)
			handle_redir_out(&cmd->redirs[i]);
		else if (cmd->redirs[i].type == REDIR_APPEND)
			handle_redir_append(&cmd->redirs[i]);
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
			handle_redir_heredoc(shell, &cmd->redirs[i]);
		i++;
	}
}
