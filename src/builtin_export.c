/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 17:02:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_get_var_name(char *arg)
{
	int		i;
	char	*name;

	i = 0;
	while (arg[i] && arg[i] != '=')
		i++;
	name = ft_substr(arg, 0, i);
	return (name);
}

static int	ft_find_env_index(t_env *env, char *key)
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

static int	process_export_arg(t_shell *shell, char *arg)
{
	int		index;
	char	*var_name;
	char	*expanded_arg;

	if (!ft_is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	if (ft_strchr(arg, '='))
	{
		expanded_arg = ft_expand_export_value(shell, arg);
		var_name = ft_get_var_name(expanded_arg);
		index = ft_find_env_index(&shell->env, var_name);
		if (index >= 0)
			ft_update_env_var(&shell->env, index, expanded_arg);
		else
			ft_add_env_var(&shell->env, expanded_arg);
		free(var_name);
		free(expanded_arg);
	}
	return (0);
}

void	ft_builtin_export(t_shell *shell, char **args)
{
	int		i;
	int		has_error;

	has_error = 0;
	if (!args[1])
	{
		print_export_sorted(&shell->env);
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		has_error |= process_export_arg(shell, args[i]);
		i++;
	}
	if (has_error)
		shell->exit_status = 1;
	else
		shell->exit_status = 0;
}
