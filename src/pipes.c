/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 18:20:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 23:20:40 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline_execve(t_shell *shell)
{
	int			i;
	int			prev_fd;
	pid_t		*child_pids;

	child_pids = init_pipeline_resources(shell);
	if (!child_pids)
		return ;
	prev_fd = -1;
	i = -1;
	while (++i < shell->command_count)
	{
		if (fork_and_handle(shell, i, &child_pids, &prev_fd) == -1)
		{
			free(child_pids);
			exit(EXIT_FAILURE);
		}
	}
	wait_all_children(shell, child_pids);
	free(child_pids);
}
