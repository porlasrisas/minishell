/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:52:10 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 23:59:59 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_pipe_if_needed(t_command *cmd, int i, int total, int pipe_fd[2])
{
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	if (cmd->pipe_after || (i < total - 1))
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (-1);
		}
	}
	return (0);
}

void	close_if_valid(int fd)
{
	if (fd != -1)
		close(fd);
}

void	setup_child_pipes(int prev_fd, t_command *cmd, int pipe_fd[2])
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (cmd->pipe_after)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);
	}
	else
	{
		close_if_valid(pipe_fd[0]);
		close_if_valid(pipe_fd[1]);
	}
}

void	parent_after_fork(pid_t pid, int *prev_fd, int pipe_fd[2], t_command *cmd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (cmd->pipe_after)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
	else
	{
		close_if_valid(pipe_fd[0]);
		close_if_valid(pipe_fd[1]);
		*prev_fd = -1;
	}
	(void)pid;
}

void	finalize_status(t_shell *shell, int last_status)
{
	int		 sig;

	if (WIFSIGNALED(last_status))
	{
		sig = WTERMSIG(last_status);
		if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		shell->exit_status = 128 + sig;
	}
	else
		shell->exit_status = WEXITSTATUS(last_status);
}

static void	execve_or_die(char *path, char **argv, char **envp)
{
	execve(path, argv, envp);
	if (errno == ENOENT)
	{
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
		exit(127);
	}
	else if (errno == EACCES)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", argv[0]);
		exit(126);
	}
	perror("execve");
	exit(1);
}

void	pipe_exec_child(t_shell *shell, t_command *cmd, int prev_fd,
	int pipe_fd[2])
{
	char	*path;

	setup_child_pipes(prev_fd, cmd, pipe_fd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (has_heredoc(cmd))
		handle_redirections_with_heredoc(cmd);
	else
		handle_redirections(cmd);
	if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
		exit(1);
	if (ft_is_builtin(cmd->args[0]))
	{
		ft_execute_builtin(shell, cmd);
		exit(shell->exit_status);
	}
	path = cmd->args[0];
	if (ft_strchr(cmd->args[0], '/') == NULL)
	{
		path = ft_resolve_command_path(shell, cmd->args[0]);
		if (!path)
			exit(127);
	}
	execve_or_die(path, cmd->args, shell->env.variables);
}
