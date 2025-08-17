/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 10:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/05 18:45:12 by Guille           ###   ########.fr       */
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

	printf("DEBUG: Agregando variable '%s'\n", var);
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
	printf("DEBUG: Variable agregada. Nuevas variables: %d\n", env->count);
}

static void	ft_update_env_var(t_env *env, int index, char *var)
{
	printf("DEBUG: Actualizando variable en índice %d con '%s'\n", index, var);
	free(env->variables[index]);
	env->variables[index] = ft_strdup(var);
	printf("DEBUG: Variable actualizada\n");
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
	
	// Expandir variables en el valor si contiene $
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

void	ft_builtin_export(t_shell *shell, char **args)
{
	int		i;
	int		index;
	char	*var_name;
	char	*expanded_arg;

	printf("DEBUG: Ejecutando export\n");
	if (!args[1])
	{
		ft_builtin_env(shell);
		return ;
	}
	i = 1;
	while (args[i])
	{
		printf("DEBUG: Procesando argumento '%s'\n", args[i]);
		if (!ft_is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(args[i], STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
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
	shell->exit_status = 0;
}

void	ft_builtin_unset(t_shell *shell, char **args)
{
	int		i;
	int		j;
	int		index;
	char	**new_vars;

	printf("DEBUG: Ejecutando unset\n");
	if (!args[1])
	{
		shell->exit_status = 0;
		return ;
	}
	i = 1;
	while (args[i])
	{
		printf("DEBUG: Eliminando variable '%s'\n", args[i]);
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
			/* Liberar la variable eliminada y el array anterior */
			free(shell->env.variables[index]);
			free(shell->env.variables);
			shell->env.variables = new_vars;
			shell->env.count--;
			printf("DEBUG: Variable eliminada. Variables restantes: %d\n", 
				   shell->env.count);
		}
		i++;
	}
	shell->exit_status = 0;
}
