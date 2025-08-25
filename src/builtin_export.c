/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_valid_identifier(char *str)
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

static void	ft_add_env_var(t_env *env, char *var)
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

static void	ft_update_env_var(t_env *env, int index, char *var)
{
	free(env->variables[index]);
	env->variables[index] = ft_strdup(var);
}

static char	*ft_expand_export_value(t_shell *shell, char *arg)
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
	if (ft_strchr(value_part, '$'))
		expanded_value = ft_expand_unquoted_token(shell, value_part);
	else
		expanded_value = ft_strdup(value_part);
	result = ft_strdup(var_name);
	result = ft_strjoin_free(result, "=");
	result = ft_strjoin_free(result, expanded_value);
	free(var_name);
	free(value_part);
	free(expanded_value);
	return (result);
}

static int	cmp_env(const void *a, const void *b)
{
	char *const *sa = (char *const *)a;
	char *const *sb = (char *const *)b;
	return (ft_strncmp(*sa, *sb, SIZE_MAX));
}

static void	print_export_sorted(t_env *env)
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
		char *eq = ft_strchr(copy[i], '=');
		if (eq)
		{
			write(1, "declare -x ", 11);
			write(1, copy[i], eq - copy[i] + 1);
			write(1, "\"", 1);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			write(1, "declare -x ", 11);
			write(1, copy[i], ft_strlen(copy[i]));
			write(1, "\n", 1);
		}
		i++;
	}
	free(copy);
}

void	ft_builtin_export(t_shell *shell, char **args)
{
	int		i;
	int		index;
	int		has_error;
	char	*var_name;
	char	*expanded_arg;

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
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			has_error = 1;
			i++;
			continue ;
		}
		if (ft_strchr(args[i], '='))
		{
			expanded_arg = ft_expand_export_value(shell, args[i]);
			var_name = ft_get_var_name(expanded_arg);
			index = ft_find_env_index(&shell->env, var_name);
			if (index >= 0)
				ft_update_env_var(&shell->env, index, expanded_arg);
			else
				ft_add_env_var(&shell->env, expanded_arg);
			free(var_name);
			free(expanded_arg);
		}
		i++;
	}
	shell->exit_status = has_error ? 1 : 0;
}

void	ft_builtin_unset(t_shell *shell, char **args)
{
	int		i;
	int		j;
	int		index;
	char	**new_vars;

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
			ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
			i++;
			continue ;
		}
		index = ft_find_env_index(&shell->env, args[i]);
		if (index >= 0)
		{
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
		i++;
	}
	shell->exit_status = 0;
}
