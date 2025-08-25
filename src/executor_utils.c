/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 16:33:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	apply_redirs(t_command *cmd)
{
	if (has_heredoc(cmd))
		handle_redirections_with_heredoc(cmd);
	else
		handle_redirections(cmd);
}

void	update_status_from_wait(t_shell *shell, int status)
{
	int	sig;

	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		shell->exit_status = 128 + sig;
	}
	else
		shell->exit_status = WEXITSTATUS(status);
}

int	ft_check_simple_command(t_shell *shell)
{
	t_command	*cmd;

	if (!shell || !shell->commands || !shell->commands[0])
		return (0);
	cmd = shell->commands[0];
	if (!cmd->args || !cmd->args[0])
		return (0);
	return (1);
}

void	ft_handle_command_not_found(t_shell *shell, char *cmd_name)
{
	fprintf(stderr, "minishell: %s: command not found\n", cmd_name);
	shell->exit_status = 127;
}
