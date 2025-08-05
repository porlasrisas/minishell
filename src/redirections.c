/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:19:10 by carbon            #+#    #+#             */
/*   Updated: 2025/08/05 18:24:11 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	handle_redirections(t_command *cmd)
{
	int	fd;
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_IN)
		{
			fd = open(cmd->redirs[i].file, O_RDONLY);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_OUT)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_APPEND)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		i++;
	}
}

void	handle_redirections_with_heredoc(t_command *cmd)
{
	int		fd;
	int		pipefd[2];
	int		i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_IN)
		{
			fd = open(cmd->redirs[i].file, O_RDONLY);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_OUT)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_APPEND)
		{
			fd = open(cmd->redirs[i].file, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(cmd->redirs[i].file);
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (cmd->redirs[i].type == REDIR_HEREDOC)
		{
			printf("DEBUG: Procesando heredoc\n");
			if (cmd->redirs[i].heredoc_content)
			{
				if (pipe(pipefd) == -1)
				{
					perror("pipe");
					exit(1);
				}
				write(pipefd[1], cmd->redirs[i].heredoc_content,
					  ft_strlen(cmd->redirs[i].heredoc_content));
				close(pipefd[1]);
				dup2(pipefd[0], STDIN_FILENO);
				close(pipefd[0]);
			}
		}
		i++;
	}
}
