/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:09:38 by carbon            #+#    #+#             */
/*   Updated: 2025/07/21 19:32:57 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline(t_shell *shell)
{
	int		i = 0;
	int		pipe_fd[2];
	int		prev_fd = -1;
	pid_t	pid;
	t_command *cmd;

	while (i < shell->command_count)
	{
		printf("Entra en execpipe\n");
		cmd = &shell->commands[i];
		if (cmd->pipe_after || (i < shell->command_count - 1))
		{
			if (pipe(pipe_fd) == -1)
			{
				printf("hace pipe\n");
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
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
			if (pipe_fd[0] != -1) close(pipe_fd[0]);
			if (pipe_fd[1] != -1) close(pipe_fd[1]);
			char *path = cmd->args[0];
			if (ft_strchr(cmd->args[0], '/') == NULL)
			{
				path = ft_resolve_command_path(shell, cmd->args[0]);
				if (!path)
				{
					perror("command not found");
					exit(127);
				}
			}
			handle_redirections(cmd);
			execve(path, cmd->args, shell->env.variables);
			perror("execve failed");
			exit(1);
		}
		else
		{
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->pipe_after)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			else
				prev_fd = -1;
		}
		i++;
	}
	wait(NULL);
}

char	*ft_resolve_command_path(t_shell *shell, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*try_path;
	int		i;

	path_env = ft_get_env(&shell->env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		try_path = ft_strjoin_free(ft_strdup(paths[i]), "/");
		try_path = ft_strjoin_free(try_path, cmd);
		if (access(try_path, X_OK) == 0)
		{
			ft_error(NULL, 1, 1, paths);
			return (try_path);
		}
		free(try_path);
		i++;
	}
	ft_error(NULL, 1, 1, paths);
	return (NULL);
}

