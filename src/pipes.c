/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/28 23:20:40 by Guille           ###   ########.fr       */
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
    int	i;
    int	status;
    int	last_status;

    i = 0;
    last_status = 0;
    while (i < count)
    {
        if (waitpid(child_pids[i], &status, 0) == -1)
        {
            if (errno != ECHILD)
                perror("waitpid");
        }
        else
        {
            if (WIFEXITED(status))
                last_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                last_status = 128 + WTERMSIG(status);
        }
        i++;
    }
    return (last_status);
}

static int	ft_create_child_process(t_shell *shell, t_command *cmd,
            int prev_fd, int *pipe_fd)
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
        if (open_pipe_if_needed(cmd, i, shell->command_count, pipe_fd) == -1)
            return (-1);
        pid = ft_create_child_process(shell, cmd, prev_fd, pipe_fd);
        if (pid == -1)
            return (ft_handle_fork_error(child_pids));
        child_pids[i] = pid;
        parent_after_fork(pid, &prev_fd, pipe_fd, cmd);
        i++;
    }
    // Cerrar el último extremo de lectura heredado en el padre
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