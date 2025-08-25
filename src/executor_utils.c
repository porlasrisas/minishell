/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 20:50:32 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

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
