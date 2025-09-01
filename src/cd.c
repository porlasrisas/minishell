/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:37:24 by carbon-m          #+#    #+#             */
/*   Updated: 2025/09/01 13:57:11 by Guille           ###   ########.fr       */
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

static int	ft_cd_to_home(t_shell *shell)
{
	char	*home;

	home = getenv("HOME");
	if (!home || chdir(home) != 0)
	{
		ft_cd_error(home);
		shell->exit_status = 1;
		return (0);
	}
	return (1);
}

static int	ft_cd_to_path(t_shell *shell, char *path)
{
	char	*expanded_path;

	expanded_path = ft_process_token_quotes(shell, path);
	if (chdir(expanded_path) != 0)
	{
		ft_cd_error(expanded_path);
		shell->exit_status = 1;
		free(expanded_path);
		return (0);
	}
	free(expanded_path);
	return (1);
}

void	ft_cd(t_shell *shell, char **args)
{
	char	*new_pwd;
	int		success;

	if (!args[1])
		success = ft_cd_to_home(shell);
	else
		success = ft_cd_to_path(shell, args[1]);
	if (success)
	{
		if (shell->env.oldpwd)
			free(shell->env.oldpwd);
		shell->env.oldpwd = shell->env.pwd;
		new_pwd = getcwd(NULL, 0);
		if (!new_pwd)
		{
			ft_cd_error(NULL);
			shell->exit_status = 1;
			return ;
		}
		shell->env.pwd = new_pwd;
		shell->exit_status = 0;
	}
}
