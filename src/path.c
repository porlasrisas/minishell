/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 20:44:55 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_resolve_command_path(t_shell *shell, char *cmd)
{
	char		**paths;
	char		*path_env;
	char		*try_path;
	int			i;
	t_format	format;

	path_env = ft_get_env(&shell->env, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		try_path = ft_strjoin_free(ft_strdup(paths[i]), "/");
		try_path = ft_strjoin_free(try_path, cmd);
		if (access(try_path, X_OK) == 0)
		{
			errno = 0;
			format.ptr = (void **)paths;
			format.ptr1 = NULL;
			format.depth = 0;
			ft_error(NULL, 1, 1, &format);
			return (try_path);
		}
		errno = 0;
		free(try_path);
		i++;
	}
	errno = 0;
	format.ptr = (void **)paths;
	format.ptr1 = NULL;
	format.depth = 0;
	ft_error(NULL, 1, 1, &format);
	return (NULL);
}
