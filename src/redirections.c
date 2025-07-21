/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:19:10 by carbon            #+#    #+#             */
/*   Updated: 2025/07/21 10:11:37 by carbon           ###   ########.fr       */
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
