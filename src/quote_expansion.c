/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/27 16:55:54 by Guille           ###   ########.fr       */
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

static char	*ft_extract_var_name(char *str, int *i)
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
	// Caso especial para $?
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*ft_expand_double_quotes(t_shell *shell, char *str)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || 
				str[i + 1] == '?' || str[i + 1] == '{'))
			{
				i++;
				var_name = ft_extract_var_name(str, &i);
				var_value = ft_expand_variable(shell, var_name);
				temp = ft_strjoin_free(result, var_value);
				result = temp;
				free(var_name);
				free(var_value);
			}
			else
			{
				temp = ft_substr(str, i, 1);
				result = ft_strjoin_free(result, temp);
				free(temp);
				i++;
			}
		}
		else if (str[i] == '\\')
		{
			temp = ft_process_escape(str, &i);
			result = ft_strjoin_free(result, temp);
			free(temp);
		}
		else
		{
			temp = ft_substr(str, i, 1);
			result = ft_strjoin_free(result, temp);
			free(temp);
			i++;
		}
	}
	return (result);
}

char	*ft_expand_unquoted_token(t_shell *shell, char *token)
{
	char	*result;
	char	*temp;
	char	*var_name;
	char	*var_value;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '$')
		{
			if (token[i + 1] && (ft_isalnum(token[i + 1]) || token[i + 1] == '_' || 
				token[i + 1] == '?' || token[i + 1] == '{'))
			{
				i++;
				var_name = ft_extract_var_name(token, &i);
				var_value = ft_expand_variable(shell, var_name);
				temp = ft_strjoin_free(result, var_value);
				result = temp;
				free(var_name);
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

static char	*ft_handle_mixed_quotes(t_shell *shell, char *token)
{
	char	*result;
	char	*temp;
	char	*segment;
	int		i;
	int		start;
	char	quote_char;

	result = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '"' || token[i] == '\'')
		{
			quote_char = token[i];
			start = ++i;
			while (token[i] && token[i] != quote_char)
				i++;
			if (token[i] == quote_char)
			{
				segment = ft_substr(token, start, i - start);
				if (quote_char == '"')
					temp = ft_expand_double_quotes(shell, segment);
				else
					temp = ft_strdup(segment);
				result = ft_strjoin_free(result, temp);
				free(segment);
				free(temp);
				i++;
			}
		}
		else
		{
			start = i;
			while (token[i] && token[i] != '"' && token[i] != '\'')
				i++;
			segment = ft_substr(token, start, i - start);
			if (ft_strchr(segment, '$'))
			{
				temp = ft_expand_unquoted_token(shell, segment);
				result = ft_strjoin_free(result, temp);
				free(temp);
			}
			else
			{
				result = ft_strjoin_free(result, segment);
			}
			free(segment);
		}
	}
	return (result);
}

char	*ft_process_token_quotes(t_shell *shell, char *token)
{
	char	*result;
	char	*content;
	int		len;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	
	// Token completamente entre comillas dobles
	if (len >= 2 && token[0] == '"' && token[len - 1] == '"' && 
		!ft_strchr(token + 1, '"'))
	{
		content = ft_substr(token, 1, len - 2);
		result = ft_expand_double_quotes(shell, content);
		free(content);
		return (result);
	}
	
	// Token completamente entre comillas simples
	if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'' && 
		!ft_strchr(token + 1, '\''))
	{
		return (ft_substr(token, 1, len - 2));
	}
	
	// Token con comillas mixtas o múltiples
	if (ft_strchr(token, '"') || ft_strchr(token, '\''))
	{
		return (ft_handle_mixed_quotes(shell, token));
	}
	
	// Token sin comillas - expandir variables si las tiene
	if (ft_strchr(token, '$'))
	{
		return (ft_expand_unquoted_token(shell, token));
	}
	
	// Token normal sin variables
	return (ft_strdup(token));
}

int	ft_is_redirection_token(char *token)
{
	if (!token)
		return (0);
	if (ft_strncmp(token, "<", ft_strlen(token)) == 0)
		return (1);
	if (ft_strncmp(token, ">", ft_strlen(token)) == 0)
		return (1);
	if (ft_strncmp(token, ">>", ft_strlen(token)) == 0)
		return (1);
	if (ft_strncmp(token, "<<", ft_strlen(token)) == 0)
		return (1);
	return (0);
}
