/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:37:24 by carbon-m          #+#    #+#             */
/*   Updated: 2025/08/25 17:46:14 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_cd_error(const char *path)
{
	const char	*error_msg;

	if (!path)
		error_msg = "cd: HOME not set\n";
	else if (errno == ENOENT)
		error_msg = ": No such file or directory\n";
	else if (errno == EACCES)
		error_msg = ": Permission denied\n";
	else if (errno == ENOTDIR)
		error_msg = ": Not a directory\n";
	else
		error_msg = ": Error\n";
	ft_putstr_fd("cd: ", 2);
	if (path)
		ft_putstr_fd((char *)path, 2);
	ft_putstr_fd((char *)error_msg, 2);
}

static int	cd_home(t_shell *shell)
{
	char	*home;

	home = getenv("HOME");
	if (!home || chdir(home) != 0)
	{
		ft_cd_error(home);
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	update_pwd(t_shell *shell)
{
	char	*new_pwd;

	if (shell->env.oldpwd)
		free(shell->env.oldpwd);
	shell->env.oldpwd = shell->env.pwd;
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		ft_cd_error(NULL);
		shell->exit_status = 1;
		return (1);
	}
	shell->env.pwd = new_pwd;
	shell->exit_status = 0;
	return (0);
}

int	ft_handle_cd(t_shell *shell)
{
	int	i;

	if (shell->tokens && shell->tokens[0] && shell->tokens[0][0] != '\0'
		&& ft_strncmp(shell->tokens[0], "cd", 2) == 0
		&& (shell->tokens[0][2] == '\0' || shell->tokens[0][2] == ' '))
	{
		i = 0;
		while (shell->tokens[i])
		{
			if ((shell->tokens[i][0] == '|' && shell->tokens[i][1] == '\0')
				|| (shell->tokens[i][0] == ';' && shell->tokens[i][1] == '\0'))
				return (0);
			i++;
		}
		ft_cd(shell, shell->tokens);
		return (1);
	}
	return (0);
}

void	ft_cd(t_shell *shell, char **args)
{
	if (!args[1])
	{
		if (cd_home(shell))
			return ;
	}
	else if (chdir(args[1]) != 0)
	{
		ft_cd_error(args[1]);
		shell->exit_status = 1;
		return ;
	}
	update_pwd(shell);
}
