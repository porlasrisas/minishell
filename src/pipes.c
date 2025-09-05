/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/02 15:16:41 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_handle_fork_error(pid_t *child_pids)
{
	perror("fork");
	free(child_pids);
	return (-1);
}

static int	ft_wait_for_children(pid_t *child_pids, int count)
{
	int	status;
	int	last_status;
	int	i;

	last_status = 0;
	if (waitpid(child_pids[count - 1], &status, 0) != -1)
	{
		if (WIFEXITED(status))
			last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			last_status = 128 + WTERMSIG(status);
	}
	i = 0;
	while (i < count - 1)
	{
		if (waitpid(child_pids[i], NULL, 0) == -1 && errno != ECHILD)
			perror("waitpid");
		i++;
	}
	return (last_status);
}

static int	ft_create_child_process(t_shell *shell, t_command *cmd, int prev_fd,
		int *pipe_fd)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		pipe_exec_child(shell, cmd, prev_fd, pipe_fd);
	return (pid);
}

static int	ft_execute_commands_loop(t_shell *shell, pid_t *child_pids)
{
	int			i;
	int			pipe_fd[2];
	int			prev_fd;
	pid_t		pid;
	t_command	*cmd;

	prev_fd = -1;
	i = 0;
	while (i < shell->command_count)
	{
		cmd = shell->commands[i];
		collect_heredocs_for_cmd(shell, cmd);
		if (open_pipe_if_needed(cmd, i, shell->command_count, pipe_fd) == -1)
			return (-1);
		pid = ft_create_child_process(shell, cmd, prev_fd, pipe_fd);
		if (pid == -1)
			return (ft_handle_fork_error(child_pids));
		child_pids[i] = pid;
		parent_after_fork(pid, &prev_fd, pipe_fd, cmd);
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	return (0);
}

void	ft_execute_pipeline_execve(t_shell *shell)
{
	pid_t	*child_pids;
	int		last_status;

	shell->exit_status = 0;
	child_pids = malloc(sizeof(pid_t) * shell->command_count);
	if (!child_pids)
		return ;
	if (ft_execute_commands_loop(shell, child_pids) == -1)
	{
		free(child_pids);
		exit(EXIT_FAILURE);
	}
	last_status = ft_wait_for_children(child_pids, shell->command_count);
	shell->exit_status = last_status;
	free(child_pids);
}
