/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_external.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 17:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 16:33:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_command_path(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = cmd->args[0];
	if (ft_strchr(cmd->args[0], '/') == NULL)
	{
		path = ft_resolve_command_path(shell, cmd->args[0]);
		if (!path)
		{
			if (errno == ENOENT)
				fprintf(stderr, "minishell: %s: command not found\n",
					cmd->args[0]);
			else if (errno == EACCES)
				fprintf(stderr, "minishell: %s: Permission denied\n",
					cmd->args[0]);
			exit(127);
		}
	}
	return (path);
}

void	exec_external_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = get_command_path(shell, cmd);
	apply_redirs(cmd);
	setup_child_signals();
	execve(path, cmd->args, shell->env.variables);
	if (errno == ENOENT)
	{
		fprintf(stderr, "minishell: %s: command not found\n",
			cmd->args[0]);
		exit(127);
	}
	if (errno == EACCES)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n",
			cmd->args[0]);
		exit(126);
	}
	perror("execve");
	exit(1);
}

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

void	handle_external_command(t_shell *shell, t_command *cmd)
{
	char	*resolved;

	if (!ft_strchr(cmd->args[0], '/'))
	{
		resolved = ft_resolve_command_path(shell, cmd->args[0]);
		if (!resolved)
		{
			ft_handle_command_not_found(shell, cmd->args[0]);
			return ;
		}
		free(resolved);
	}
	execute_external_command(shell, cmd);
}
