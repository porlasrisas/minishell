/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:24:10 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_find_env_index(t_env *env, char *key)
{
	int	i;
	int	key_len;

	if (!key || !env->variables)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (i < env->count)
	{
		if (ft_strncmp(env->variables[i], key, key_len) == 0
			&& (env->variables[i][key_len] == '='
			|| env->variables[i][key_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

static void	remove_env_variable(t_shell *shell, int index)
{
	char	**new_vars;
	int		j;

	new_vars = malloc(sizeof(char *) * shell->env.count);
	if (!new_vars)
		return ;
	j = 0;
	while (j < shell->env.count)
	{
		if (j < index)
			new_vars[j] = shell->env.variables[j];
		else if (j > index)
			new_vars[j - 1] = shell->env.variables[j];
		j++;
	}
	free(shell->env.variables[index]);
	free(shell->env.variables);
	shell->env.variables = new_vars;
	shell->env.count--;
}

static void	handle_invalid_identifier(t_shell *shell, char *arg)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	shell->exit_status = 1;
}

void	ft_builtin_unset(t_shell *shell, char **args)
{
	int		i;
	int		index;

	if (!args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]))
		{
			handle_invalid_identifier(shell, args[i]);
			i++;
			continue ;
		}
		index = ft_find_env_index(&shell->env, args[i]);
		if (index >= 0)
			remove_env_variable(shell, index);
		i++;
	}
	shell->exit_status = 0;
}
