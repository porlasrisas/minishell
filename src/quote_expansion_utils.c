/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:45:00 by Guille           #+#    #+#             */
/*   Updated: 2025/08/25 18:45:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*ft_process_escape(char *str, int *i)
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

static char	*process_variable_in_quotes(t_shell *shell, char *str, int *i)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = ft_extract_var_name(str, i);
	var_value = ft_expand_variable(shell, var_name);
	free(var_name);
	return (var_value);
}

char	*ft_expand_double_quotes(t_shell *shell, char *str)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '{' || str[i + 1] == '?'))
            temp = process_variable_in_quotes(shell, str, &i);
        else if (str[i] == '$')
        {
            // Lone $: treat as literal
            temp = ft_substr(str, i, 1);
            i++;
        }
        else if (str[i] == '\\')
            temp = ft_process_escape(str, &i);
        else
        {
            temp = ft_substr(str, i, 1);
            i++;
        }
        result = ft_strjoin_free(result, temp);
        free(temp);
    }
    return (result);
}

static char	*expand_var_in_unquoted(t_shell *shell, char *token, int *i)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = ft_extract_var_name(token, i);
	var_value = ft_expand_variable(shell, var_name);
	free(var_name);
	return (var_value);
}

char	*build_unquoted_result(t_shell *shell, char *token)
{
	char	*result;
	char	*temp;
	char	*var_value;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '$' && token[i + 1])
		{
			var_value = expand_var_in_unquoted(shell, token, &i);
			temp = ft_strjoin_free(result, var_value);
			result = temp;
			free(var_value);
		}
		else
		{
			temp = ft_substr(token, i, 1);
			result = ft_strjoin_free(result, temp);
			free(temp);
			i++;
		}
	}
	return (result);
}
