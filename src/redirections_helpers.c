/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:15:00 by Guille           #+#    #+#             */
/*   Updated: 2025/08/25 13:15:00 by Guille           ###   ########.fr       */
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

void	process_heredoc_content(t_redirection *redir)
{
	int	pipefd[2];

	if (!redir->heredoc_content)
		redir->heredoc_content = read_heredoc_content(redir->file);
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
	}
}

void	handle_single_redirection(t_redirection *redir)
{
	if (redir->type == REDIR_IN)
		handle_redir_in(redir);
	else if (redir->type == REDIR_OUT)
		handle_redir_out(redir);
	else if (redir->type == REDIR_APPEND)
		handle_redir_append(redir);
	else if (redir->type == REDIR_HEREDOC)
		process_heredoc_content(redir);
}
