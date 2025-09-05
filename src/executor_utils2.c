/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 18:20:00 by Guille           #+#    #+#             */
/*   Updated: 2025/09/01 18:20:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

int	is_directory(const char *path)
{
	struct stat	st;

	if (!path)
		return (0);
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

void	print_err_3(const char *a, const char *b, const char *c)
{
	if (a)
		ft_putstr_fd((char *)a, 2);
	if (b)
		ft_putstr_fd((char *)b, 2);
	if (c)
		ft_putstr_fd((char *)c, 2);
}

static void	handle_execve_errors(t_shell *shell, t_command *cmd, char *path)
{
	(void)shell;
	if (errno == ENOENT)
	{
		print_err_3("minishell: ", cmd->args[0], NULL);
		if (path && ft_strchr(path, '/'))
			ft_putstr_fd(": No such file or directory\n", 2);
		else
			ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	if (errno == EACCES)
	{
		if (path && is_directory(path))
		{
			print_err_3("minishell: ", cmd->args[0], ": is a directory\n");
			exit(126);
		}
		print_err_3("minishell: ", cmd->args[0], ": Permission denied\n");
		exit(126);
	}
	perror("execve");
	exit(1);
}

void	exec_external_in_child(t_shell *shell, t_command *cmd)
{
	char	*path;

	path = cmd->args[0];
	if (ft_strchr(cmd->args[0], '/') == NULL)
	{
		path = ft_resolve_command_path(shell, cmd->args[0]);
		if (!path)
		{
			print_err_3("minishell: ", cmd->args[0], ": command not found\n");
			exit(127);
		}
	}
	if (path && is_directory(path))
	{
		print_err_3("minishell: ", cmd->args[0], ": is a directory\n");
		exit(126);
	}
	setup_child_signals();
	execve(path, cmd->args, shell->env.variables);
	handle_execve_errors(shell, cmd, path);
}
