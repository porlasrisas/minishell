/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 21:45:00 by Guille           #+#    #+#             */
/*   Updated: 2025/09/02 03:38:32 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_expand_variable(t_shell *shell, char *var_name)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (var_name[0] == '?' && var_name[1] == '\0')
		return (ft_itoa(shell->exit_status));
	value = ft_get_env(&shell->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}

char	*ft_process_escape(char *str, int *i)
{
	char	*result;

	if (str[*i + 1] == '\0')
		return (ft_strdup("\\"));
	if (str[*i + 1] == 'n')
	{
		*i += 2;
		return (ft_strdup("\n"));
	}
	if (str[*i + 1] == 't')
	{
		*i += 2;
		return (ft_strdup("\t"));
	}
	result = ft_substr(str, *i + 1, 1);
	*i += 2;
	return (result);
}

char	*ft_extract_var_name(char *str, int *i)
{
	int		start;
	char	*var_name;

	start = *i;
	if (str[*i] == '{')
	{
		(*i)++;
		start = *i;
		while (str[*i] && str[*i] != '}')
			(*i)++;
		var_name = ft_substr(str, start, *i - start);
		if (str[*i] == '}')
			(*i)++;
		return (var_name);
	}
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*ft_append_char(char *result, char c)
{
	char	temp[2];

	temp[0] = c;
	temp[1] = '\0';
	return (ft_strjoin_free(result, temp));
}

char	*ft_expand_variable_in_string(t_shell *shell, char *str, int *i)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = ft_extract_var_name(str, i);
	var_value = ft_expand_variable(shell, var_name);
	free(var_name);
	return (var_value);
}
