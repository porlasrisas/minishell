/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 20:25:00 by Guille           #+#    #+#             */
/*   Updated: 2025/09/01 20:25:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmp_env(const void *a, const void *b)
{
	char *const	*sa;
	char *const	*sb;

	sa = (char *const *)a;
	sb = (char *const *)b;
	return (ft_strncmp(*sa, *sb, SIZE_MAX));
}

static void	print_single_export(char *var)
{
	char	*eq;

	eq = ft_strchr(var, '=');
	ft_putstr_fd("declare -x ", 1);
	if (eq)
	{
		write(1, var, eq - var + 1);
		ft_putchar_fd('"', 1);
		ft_putstr_fd(eq + 1, 1);
		ft_putstr_fd("\"\n", 1);
	}
	else
	{
		ft_putstr_fd(var, 1);
		ft_putchar_fd('\n', 1);
	}
}

void	print_export_sorted(t_env *env)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (env->count + 1));
	if (!copy)
		return ;
	i = 0;
	while (i < env->count)
	{
		copy[i] = env->variables[i];
		i++;
	}
	copy[i] = NULL;
	qsort(copy, env->count, sizeof(char *), cmp_env);
	i = 0;
	while (i < env->count)
	{
		print_single_export(copy[i]);
		i++;
	}
	free(copy);
}

static void	handle_export_assignment(t_shell *shell, char *arg)
{
	char	*expanded_arg;
	char	*var_name;
	int		index;

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

void	ft_builtin_export(t_shell *shell, char **args)
{
	int	i;
	int	has_error;

	if (!args[1])
	{
		print_export_sorted(&shell->env);
		shell->exit_status = 0;
		return ;
	}
	has_error = 0;
	i = 1;
	while (args[i])
	{
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			has_error = 1;
		}
		else if (ft_strchr(args[i], '='))
			handle_export_assignment(shell, args[i]);
		i++;
	}
	shell->exit_status = has_error;
}
