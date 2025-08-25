/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:25:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_paths(char **paths)
{
	t_format	format;

	errno = 0;
	format.ptr = (void **)paths;
	format.ptr1 = NULL;
	format.depth = 0;
	ft_error(NULL, 1, 1, &format);
}

static char	*try_path_access(char *dir, char *cmd)
{
	char	*try_path;

	try_path = ft_strjoin_free(ft_strdup(dir), "/");
	try_path = ft_strjoin_free(try_path, cmd);
	if (access(try_path, X_OK) == 0)
		return (try_path);
	errno = 0;
	free(try_path);
	return (NULL);
}

char	*ft_resolve_command_path(t_shell *shell, char *cmd)
{
	char		**paths;
	char		*path_env;
	char		*result;
	int			i;

	path_env = ft_get_env(&shell->env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		result = try_path_access(paths[i], cmd);
		if (result)
		{
			cleanup_paths(paths);
			return (result);
		}
		i++;
	}
	cleanup_paths(paths);
	return (NULL);
}
