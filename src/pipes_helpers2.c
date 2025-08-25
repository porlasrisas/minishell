/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_helpers2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:21:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	finalize_status(t_shell *shell, int last_status)
{
	int	sig;

	if (WIFSIGNALED(last_status))
	{
		sig = WTERMSIG(last_status);
		if (sig == SIGQUIT)
			write(2, "Quit: 3\n", 8);
		shell->exit_status = 128 + sig;
	}
	else
		shell->exit_status = WEXITSTATUS(last_status);
}

void	execve_or_die(char *path, char **argv, char **envp)
{
	execve(path, argv, envp);
	if (errno == ENOENT)
	{
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
		exit(127);
	}
	else if (errno == EACCES)
	{
		fprintf(stderr, "minishell: %s: Permission denied\n", argv[0]);
		exit(126);
	}
	perror("execve");
	exit(1);
}
