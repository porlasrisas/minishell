/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:53:24 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_expand_variable(t_shell *shell, char *var_name)
{
	char	*value;

	if (!var_name)
		return (ft_strdup(""));
	if (ft_strncmp(var_name, "?", 1) == 0)
		return (ft_itoa(shell->exit_status));
	value = ft_get_env(&shell->env, var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
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

char	*ft_expand_unquoted_token(t_shell *shell, char *token)
{
	return (build_unquoted_result(shell, token));
}

char	*ft_process_token_quotes(t_shell *shell, char *token)
{
	return (process_token_by_type(shell, token));
}
