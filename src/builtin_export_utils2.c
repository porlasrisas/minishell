/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 16:08:38 by Guille           ###   ########.fr       */
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
	free(env->variables);
	env->variables = new_vars;
	env->count++;
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
