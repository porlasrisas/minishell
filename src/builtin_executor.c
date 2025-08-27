/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/27 17:52:14 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_exec_error_and_exit(char **argv, int code)
{
	if (code == 127)
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
	else if (code == 126)
		fprintf(stderr, "minishell: %s: Permission denied\n", argv[0]);
	exit(code);
}

static void	exec_external_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = cmd->args[0];
	if (ft_strchr(cmd->args[0], '/') == NULL)
	{
		path = ft_resolve_command_path(shell, cmd->args[0]);
		if (!path)
			print_exec_error_and_exit(cmd->args, 127);
	}
	apply_redirs(cmd);
	setup_child_signals();
	execve(path, cmd->args, shell->env.variables);
	if (errno == ENOENT)
		print_exec_error_and_exit(cmd->args, 127);
	if (errno == EACCES)
		print_exec_error_and_exit(cmd->args, 126);
	perror("execve");
	exit(1);
}

static void	execute_external_command(t_shell *shell, t_command *cmd)
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
		exec_external_in_child(shell, cmd);
	else
	{
		waitpid(pid, &status, 0);
		update_status_from_wait(shell, status);
	}
}

static int	exec_builtin_with_redirs(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	else if (pid == 0)
	{
		apply_redirs(cmd);
		setup_child_signals();
		ft_execute_builtin(shell, cmd);
		exit(shell->exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		update_status_from_wait(shell, status);
	}
	return (1);
}

void	ft_execute_simple_command(t_shell *shell)
{
	t_command	*cmd;

	if (!shell || !shell->commands || !shell->commands[0])
		return ;
	cmd = shell->commands[0];
	if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
	{
		shell->exit_status = 0;  // Variable vacía da exit code 0
		return ;
	}
	if (ft_is_builtin(cmd->args[0]) && cmd->redir_count == 0)
	{
		ft_execute_builtin(shell, cmd);
		return ;
	}
	if (ft_is_builtin(cmd->args[0]) && cmd->redir_count > 0)
	{
		if (exec_builtin_with_redirs(shell, cmd))
			return ;
	}
	/* Si no hay barra y PATH no contiene el comando, devolver 127 sin forkar */
	if (!ft_strchr(cmd->args[0], '/'))
	{
		char *resolved = ft_resolve_command_path(shell, cmd->args[0]);
		if (!resolved)
		{
			fprintf(stderr, "minishell: %s: command not found\n", cmd->args[0]);
			shell->exit_status = 127;
			return ;
		}
		free(resolved);
	}
	execute_external_command(shell, cmd);
}
