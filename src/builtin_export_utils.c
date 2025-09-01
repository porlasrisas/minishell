/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 19:35:00 by Guille           #+#    #+#             */
/*   Updated: 2025/09/01 19:35:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

char	*ft_get_var_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	return (name);
}

int	ft_find_env_index(t_env *env, char *key)
{
	int		i;
	size_t	key_len;

	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->variables[i], key, key_len) == 0
			&& env->variables[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	ft_add_env_var(t_env *env, char *var)
{
	char	**new_vars;
	int		i;

	new_vars = malloc(sizeof(char *) * (env->count + 2));
	if (!new_vars)
		return ;
	i = 0;
	while (i < env->count)
	{
		new_vars[i] = env->variables[i];
		i++;
	}
	new_vars[i] = ft_strdup(var);
	new_vars[i + 1] = NULL;
	if (env->variables)
		free(env->variables);
	env->variables = new_vars;
	env->count++;
}

void	ft_update_env_var(t_env *env, int index, char *var)
{
	free(env->variables[index]);
	env->variables[index] = ft_strdup(var);
}
