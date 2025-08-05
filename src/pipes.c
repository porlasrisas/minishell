/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 21:09:38 by carbon            #+#    #+#             */
/*   Updated: 2025/07/22 00:26:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline_execve(t_shell *shell)
{
	int		i = 0;
	int		pipe_fd[2];
	int		prev_fd = -1;
	pid_t	pid;
	pid_t	*child_pids;
	t_command *cmd;
	int		status;

	child_pids = malloc(sizeof(pid_t) * shell->command_count);
	if (!child_pids)
		return;
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	while (i < shell->command_count)
	{
		cmd = shell->commands[i];
		pipe_fd[0] = -1;
		pipe_fd[1] = -1;
		if (cmd->pipe_after || (i < shell->command_count - 1))
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe");
				free(child_pids);
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			free(child_pids);
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
			else
			{
				if (pipe_fd[0] != -1) close(pipe_fd[0]);
				if (pipe_fd[1] != -1) close(pipe_fd[1]);
			}
			
			// Debug: verificar comando
			printf("DEBUG CHILD: Ejecutando comando='%s'\n", cmd->args[0] ? cmd->args[0] : "NULL");
			
			// Verificar si el comando está vacío
			if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
			{
				printf("DEBUG: Comando vacío en pipeline, saliendo\n");
				exit(1);
			}
			
			// Verificar si es un builtin
			if (ft_is_builtin(cmd->args[0]))
			{
				ft_putstr_fd("DEBUG: Builtin detectado en pipeline: '", 2);
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd("'\n", 2);
				ft_execute_builtin(shell, cmd);
				exit(0); // Salir exitosamente del proceso hijo
			}
			
			char *path = cmd->args[0];
			if (ft_strchr(cmd->args[0], '/') == NULL)
			{
				printf("DEBUG: Resolviendo path para '%s'\n", cmd->args[0]);
				path = ft_resolve_command_path(shell, cmd->args[0]);
				printf("DEBUG: Path resuelto='%s'\n", path ? path : "NULL");
				if (!path)
				{
					perror("command not found");
					exit(127);
				}
			}
			handle_redirections(cmd);
			
			// Debug: verificar argumentos antes de execve
			printf("DEBUG: execve path='%s'\n", path);
			int j = 0;
			while (cmd->args[j])
			{
				printf("DEBUG: args[%d]='%s'\n", j, cmd->args[j]);
				j++;
			}
			
			execve(path, cmd->args, shell->env.variables);
			perror("execve failed");
			exit(1);
		}
		else
		{
			child_pids[i] = pid;
			if (prev_fd != -1)
				close(prev_fd);
			if (cmd->pipe_after)
			{
				close(pipe_fd[1]);
				prev_fd = pipe_fd[0];
			}
			else
			{
				if (pipe_fd[0] != -1) close(pipe_fd[0]);
				if (pipe_fd[1] != -1) close(pipe_fd[1]);
				prev_fd = -1;
			}
		}
		i++;
	}
	
	// Esperar a todos los procesos hijo
	i = 0;
	while (i < shell->command_count)
	{
		if (waitpid(child_pids[i], &status, 0) == -1)
		{
			if (errno != ECHILD)
				perror("waitpid");
		}
		i++;
	}
	free(child_pids);
}


char	*ft_resolve_command_path(t_shell *shell, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*try_path;
	int		i;

	printf("DEBUG ft_resolve_command_path: buscando '%s'\n", cmd);
	path_env = ft_get_env(&shell->env, "PATH");
	printf("DEBUG: PATH='%s'\n", path_env ? path_env : "NULL");
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
		printf("DEBUG: probando path='%s'\n", try_path);
		if (access(try_path, X_OK) == 0)
		{
			printf("DEBUG: ENCONTRADO path='%s'\n", try_path);
			errno = 0; // Limpiar errno antes de llamar ft_error
			t_format format;
			format.ptr = (void **)paths;
			format.ptr1 = NULL;
			format.depth = 0;
			ft_error(NULL, 1, 1, &format);
			return (try_path);
		}
		errno = 0; // Limpiar errno después de cada access() que falle
		free(try_path);
		i++;
	}
	printf("DEBUG: NO ENCONTRADO comando '%s'\n", cmd);
	errno = 0; // Limpiar errno antes de llamar ft_error
	t_format format;
	format.ptr = (void **)paths;
	format.ptr1 = NULL;
	format.depth = 0;
	ft_error(NULL, 1, 1, &format);
	return (NULL);
}

