/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:15:00 by Guille           #+#    #+#             */
/*   Updated: 2025/09/02 03:27:55 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_remove_env_at_index(t_env *env, int index)
{
	free(env->variables[index]);
	while (index < env->count - 1)
	{
		env->variables[index] = env->variables[index + 1];
		index++;
	}
	env->count--;
}

static void	print_unset_error(char *arg)
{
	ft_putstr_fd("minishell: unset: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

static int	handle_single_unset(t_shell *shell, char *arg)
{
	int	index;

	if (!ft_is_valid_identifier(arg))
	{
		print_unset_error(arg);
		return (1);
	}
	index = ft_find_env_index(&shell->env, arg);
	if (index >= 0)
		ft_remove_env_at_index(&shell->env, index);
	return (0);
}

void	ft_builtin_unset(t_shell *shell, char **args)
{
	int	i;
	int	has_error;

	if (!args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	has_error = 0;
	i = 1;
	while (args[i])
	{
		if (handle_single_unset(shell, args[i]))
			has_error = 1;
		i++;
	}
	shell->exit_status = has_error;
}

char	*ft_expand_export_value(t_shell *shell, char *arg)
{
	char	*equals_pos;
	char	*var_name;
	char	*value_part;
	char	*expanded_value;
	char	*result;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		return (ft_strdup(arg));
	var_name = ft_substr(arg, 0, equals_pos - arg);
	value_part = ft_strdup(equals_pos + 1);
	expanded_value = ft_process_token_quotes(shell, value_part);
	result = ft_strjoin_free(ft_strjoin_free(ft_strdup(var_name), "="),
			expanded_value);
	free(var_name);
	free(value_part);
	free(expanded_value);
	return (result);
}
