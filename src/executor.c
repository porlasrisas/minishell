/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:45:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 23:59:59 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	executor_run(t_shell *shell)
{
	if (shell->command_count == 1 && !shell->commands[0]->pipe_after)
		ft_execute_simple_command(shell);
	else if (shell->command_count > 1)
		ft_execute_pipeline_execve(shell);
}
