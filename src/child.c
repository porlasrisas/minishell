/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:31:54 by carbon-m          #+#    #+#             */
/*   Updated: 2025/06/16 10:36:54 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child(t_shell shell, int num)
{
	pid_t	pid;
	int		fd[2];

	if (shell.commands->args[num] == 0)
	{
		ft_putstr_fd("zsh: permission denied:\n", 2);
		exit(127);
	}
	if (pipe(fd) == -1)
		error_msg("Failed to create the pipe");
	pid = fork();
	if (pid == -1)
		error_msg("Failed to create the fork");
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		process(shell, num);
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		waitpid(pid, NULL, 0);
	}
}
