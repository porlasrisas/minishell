/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:52:10 by Guille            #+#    #+#             */
/*   Updated: 2025/09/02 13:37:21 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_pipe_if_needed(t_command *cmd, int i, int total, int pipe_fd[2])
{
	(void)cmd;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (i < total - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

void	setup_child_pipes(int prev_fd, t_command *cmd, int pipe_fd[2])
{
	(void)cmd;
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (pipe_fd[1] != -1)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	if (pipe_fd[0] != -1)
		close(pipe_fd[0]);
}

void	parent_after_fork(pid_t pid, int *prev_fd, int pipe_fd[2],
		t_command *cmd)
{
	(void)pid;
	(void)cmd;
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
		return ;
	}
	*prev_fd = -1;
}

static void	close_extra_fds(void)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		close(fd);
		fd++;
	}
}

void	pipe_exec_child(t_shell *shell, t_command *cmd, int prev_fd,
		int pipe_fd[2])
{
	char	*arg0;

	setup_child_pipes(prev_fd, cmd, pipe_fd);
	arg0 = NULL;
	if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
		exit(1);
	arg0 = cmd->args[0];
	if (ft_is_builtin(arg0))
	{
		apply_redirs(cmd);
		setup_child_signals();
		close_extra_fds();
		ft_execute_builtin(shell, cmd);
		exit(shell->exit_status);
	}
	close_extra_fds();
	exec_external_in_child(shell, cmd);
}
