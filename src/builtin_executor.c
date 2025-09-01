/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/01 13:37:45 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_external_command(t_shell *shell, t_command *cmd)
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

static int	handle_builtin_command(t_shell *shell, t_command *cmd)
{
	if (ft_is_builtin(cmd->args[0]) && cmd->redir_count == 0)
	{
		ft_execute_builtin(shell, cmd);
		return (1);
	}
	if (ft_is_builtin(cmd->args[0]) && cmd->redir_count > 0)
	{
		if (exec_builtin_with_redirs(shell, cmd))
			return (1);
	}
	return (0);
}

static int	validate_external_command(t_shell *shell, t_command *cmd)
{
	char	*resolved;

	if (!ft_strchr(cmd->args[0], '/'))
	{
		resolved = ft_resolve_command_path(shell, cmd->args[0]);
		if (!resolved)
		{
			print_err_3("minishell: ", cmd->args[0], ": command not found\n");
			shell->exit_status = 127;
			return (0);
		}
		free(resolved);
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
		shell->exit_status = 0;
		return ;
	}
	if (handle_builtin_command(shell, cmd))
		return ;
	if (!validate_external_command(shell, cmd))
		return ;
	execute_external_command(shell, cmd);
}
