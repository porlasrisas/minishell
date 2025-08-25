/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtings.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 19:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 13:01:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "cd", 2) == 0 && (cmd[2] == '\0' || cmd[2] == ' '))
		return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && (cmd[6] == '\0' || cmd[6] == ' '))
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && (cmd[5] == '\0' || cmd[5] == ' '))
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (1);
	return (0);
}

static int	exec_dispatch1(t_shell *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->args[0], "cd", 2) == 0
		&& (cmd->args[0][2] == '\0' || cmd->args[0][2] == ' '))
	{
		ft_cd(shell, cmd->args);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "pwd", 3) == 0
		&& (cmd->args[0][3] == '\0' || cmd->args[0][3] == ' '))
	{
		ft_builtin_pwd();
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "echo", 4) == 0
		&& (cmd->args[0][4] == '\0' || cmd->args[0][4] == ' '))
	{
		ft_builtin_echo(cmd->args);
		return (1);
	}
	return (0);
}

static int	exec_dispatch2(t_shell *shell, t_command *cmd)
{
	if (ft_strncmp(cmd->args[0], "env", 3) == 0
		&& (cmd->args[0][3] == '\0' || cmd->args[0][3] == ' '))
	{
		ft_builtin_env(shell);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "export", 6) == 0
		&& (cmd->args[0][6] == '\0' || cmd->args[0][6] == ' '))
	{
		ft_builtin_export(shell, cmd->args);
		return (1);
	}
	if (ft_strncmp(cmd->args[0], "unset", 5) == 0
		&& (cmd->args[0][5] == '\0' || cmd->args[0][5] == ' '))
	{
		ft_builtin_unset(shell, cmd->args);
		return (1);
	}
	return (0);
}

void	ft_execute_builtin(t_shell *shell, t_command *cmd)
{
	if (exec_dispatch1(shell, cmd))
		return ;
	if (exec_dispatch2(shell, cmd))
		return ;
	if (ft_strncmp(cmd->args[0], "exit", 4) == 0
		&& (cmd->args[0][4] == '\0' || cmd->args[0][4] == ' '))
	{
		ft_builtin_exit(shell, cmd->args);
		return ;
	}
}

int	has_heredoc(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		if (cmd->redirs[i].type == REDIR_HEREDOC)
			return (1);
		i++;
	}
	return (0);
}
