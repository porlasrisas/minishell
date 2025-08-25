/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 16:31:06 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (!ft_check_simple_command(shell))
		return ;
	cmd = shell->commands[0];
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
	handle_external_command(shell, cmd);
}
