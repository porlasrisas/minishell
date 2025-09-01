/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/02 04:05:22 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_split(char **arr)
{
	int	k;

	if (!arr)
		return ;
	k = 0;
	while (arr[k])
	{
		free(arr[k]);
		k++;
	}
	free(arr);
}

static char	*join_path(const char *dir, const char *cmd)
{
	char	*tmp;
	char	*res;

	tmp = ft_strjoin_free(ft_strdup(dir), "/");
	res = ft_strjoin_free(tmp, cmd);
	return (res);
}

char	*ft_resolve_command_path(t_shell *shell, char *cmd)
{
	char	**paths;
	char	*path_env;
	char	*try_path;
	int		i;

	path_env = ft_get_env(&shell->env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		try_path = join_path(paths[i], cmd);
		if (access(try_path, X_OK) == 0)
		{
			free_split(paths);
			return (try_path);
		}
		free(try_path);
		i++;
	}
	free_split(paths);
	return (NULL);
}
