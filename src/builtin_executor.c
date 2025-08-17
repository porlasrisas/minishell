/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/17 14:05:45 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_handle_child_process(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = cmd->args[0];
	if (ft_strchr(cmd->args[0], '/') == NULL)
	{
		path = ft_resolve_command_path(shell, cmd->args[0]);
		if (!path)
		{
			printf("DEBUG: Comando '%s' no encontrado\n", cmd->args[0]);
			exit(127);
		}
	}
	// Usar la función con heredoc para todas las redirecciones
	if (has_heredoc(cmd))
		handle_redirections_with_heredoc(cmd);
	else
		handle_redirections(cmd);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
	execve(path, cmd->args, shell->env.variables);
	perror("execve failed");
	exit(1);
}

static void	ft_execute_external_command(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	else if (pid == 0)
		ft_handle_child_process(shell, cmd);
	else
	{
		waitpid(pid, &status, 0);
		shell->exit_status = WEXITSTATUS(status);
	}
}

void	ft_execute_simple_command(t_shell *shell)
{
	t_command	*cmd;
	pid_t		pid;
	int			status;

	if (!shell || !shell->commands || !shell->commands[0])
		return ;
	cmd = shell->commands[0];
	if (!cmd->args || !cmd->args[0])
	{
		printf("DEBUG: Comando vacío, ignorando\n");
		return ;
	}
	printf("DEBUG: Ejecutando comando simple '%s'\n", cmd->args[0]);
	
	// Si hay redirecciones y es un builtin, necesitamos fork
	if (ft_is_builtin(cmd->args[0]) && cmd->redir_count == 0)
	{
		ft_execute_builtin(shell, cmd);
		return ;
	}
	else if (ft_is_builtin(cmd->args[0]) && cmd->redir_count > 0)
	{
		// Fork para builtin con redirecciones
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return ;
		}
		else if (pid == 0)
		{
			if (has_heredoc(cmd))
				handle_redirections_with_heredoc(cmd);
			else
				handle_redirections(cmd);
			ft_execute_builtin(shell, cmd);
			exit(shell->exit_status);
		}
		else
		{
			waitpid(pid, &status, 0);
			shell->exit_status = WEXITSTATUS(status);
		}
		return ;
	}
	ft_execute_external_command(shell, cmd);
}
