/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 23:20:40 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline_execve(t_shell *shell)
{
	int		i;
	int		pipe_fd[2];
	int		prev_fd;
	pid_t	pid;
	pid_t	*child_pids;
	t_command	*cmd;
	int		status;
	int		last_status;

	child_pids = malloc(sizeof(pid_t) * shell->command_count);
	if (!child_pids)
		return ;
	prev_fd = -1;
	i = 0;
	while (i < shell->command_count)
	{
		cmd = shell->commands[i];
		if (open_pipe_if_needed(cmd, i, shell->command_count, pipe_fd) == -1)
		{
			free(child_pids);
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(child_pids);
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			pipe_exec_child(shell, cmd, prev_fd, pipe_fd);
		else
		{
			child_pids[i] = pid;
			parent_after_fork(pid, &prev_fd, pipe_fd, cmd);
		}
		i++;
	}
	last_status = 0;
	i = 0;
	while (i < shell->command_count)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
		{
			if (errno != ECHILD)
				perror("waitpid");
		}
		if (i == shell->command_count - 1)
			last_status = status;
		i++;
	}
	finalize_status(shell, last_status);
	free(child_pids);
}

