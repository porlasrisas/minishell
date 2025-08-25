/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:21:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

pid_t	*init_pipeline_resources(t_shell *shell)
{
	pid_t	*child_pids;

	child_pids = malloc(sizeof(pid_t) * shell->command_count);
	return (child_pids);
}

void	wait_all_children(t_shell *shell, pid_t *child_pids)
{
	int	i;
	int	status;
	int	last_status;

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
}

pid_t	create_child_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror("fork");
	return (pid);
}

void	handle_child_in_pipeline(t_shell *shell, int i, int prev_fd,
			int pipe_fd[2])
{
	pipe_exec_child(shell, shell->commands[i], prev_fd, pipe_fd);
}

int	fork_and_handle(t_shell *shell, int i, pid_t **child_pids, int *prev_fd)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (open_pipe_if_needed(shell->commands[i], i, shell->command_count,
			pipe_fd) == -1)
		return (-1);
	pid = create_child_process();
	if (pid == -1)
		return (-1);
	else if (pid == 0)
		handle_child_in_pipeline(shell, i, *prev_fd, pipe_fd);
	else
	{
		(*child_pids)[i] = pid;
		parent_after_fork(pid, prev_fd, pipe_fd, shell->commands[i]);
	}
	return (0);
}
