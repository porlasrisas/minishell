/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/22 00:04:50 by Guille           ###   ########.fr       */
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
	handle_redirections(cmd);
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

	if (!shell || !shell->commands || !shell->commands[0])
		return ;
	cmd = shell->commands[0];
	if (!cmd->args || !cmd->args[0])
	{
		printf("DEBUG: Comando vacío, ignorando\n");
		return ;
	}
	printf("DEBUG: Ejecutando comando simple '%s'\n", cmd->args[0]);
	if (ft_is_builtin(cmd->args[0]))
	{
		ft_execute_builtin(shell, cmd);
		return ;
	}
	ft_execute_external_command(shell, cmd);
}
