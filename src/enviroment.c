/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enviroment.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 20:15:09 by carbon            #+#    #+#             */
/*   Updated: 2025/07/18 20:15:23 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**copy_env(char **envp, int *count)
{
	int i = 0;
	while (envp[i])
		i++;
	*count = i;
	char **env = malloc(sizeof(char *) * (i + 1));
	if (!env)
		return (NULL);
	int j = 0;
	while (j < i)
	{
		env[j] = ft_strdup(envp[j]);
		j++;
	}
	env[i] = NULL;
	return (env);
}

char	*get_env_value(t_env *env, const char *search)
{
	int len = ft_strlen(search);
	int i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->variables[i], search, len) == 0 && env->variables[i][len] == '=')
			return (env->variables[i] + len + 1);
		i++;
	}
	return (NULL);
}
