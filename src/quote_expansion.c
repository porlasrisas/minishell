/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 01:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/02 03:45:50 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	qe_is_var_start(char *s, int i)
{
	return (s[i] == '$' && s[i + 1] && (ft_isalnum(s[i + 1]) || s[i + 1] == '_'
			|| s[i + 1] == '?' || s[i + 1] == '{'));
}

static char	*qe_expand_segment(t_shell *shell, char *str, int quoted)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (qe_is_var_start(str, i))
		{
			tmp = ft_expand_variable_in_string(shell, str, &i);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
		else if (quoted && str[i] == '\\')
		{
			tmp = ft_process_escape(str, &i);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
		else
			result = ft_append_char(result, str[i++]);
	}
	return (result);
}

static char	*qe_collect_quoted(t_shell *shell, char *token, int *i)
{
	char	quote;
	int		start;
	char	*seg;
	char	*tmp;

	quote = token[*i];
	(*i)++;
	start = *i;
	while (token[*i] && token[*i] != quote)
		(*i)++;
	seg = ft_substr(token, start, *i - start);
	if (quote == '"')
		tmp = qe_expand_segment(shell, seg, 1);
	else
		tmp = ft_strdup(seg);
	free(seg);
	if (token[*i] == quote)
		(*i)++;
	return (tmp);
}

static char	*qe_collect_plain(t_shell *shell, char *token, int *i)
{
	int		start;
	char	*seg;
	char	*tmp;

	start = *i;
	while (token[*i] && token[*i] != '"' && token[*i] != '\'')
		(*i)++;
	seg = ft_substr(token, start, *i - start);
	tmp = qe_expand_segment(shell, seg, 0);
	free(seg);
	return (tmp);
}

char	*ft_process_token_quotes(t_shell *shell, char *token)
{
	char	*res;
	char	*tmp;
	int		i;

	if (!token)
		return (NULL);
	if (!ft_strchr(token, '"') && !ft_strchr(token, '\''))
	{
		if (ft_strchr(token, '$'))
			return (qe_expand_segment(shell, token, 0));
		return (ft_strdup(token));
	}
	res = ft_strdup("");
	i = 0;
	while (token[i])
	{
		if (token[i] == '"' || token[i] == '\'')
			tmp = qe_collect_quoted(shell, token, &i);
		else
			tmp = qe_collect_plain(shell, token, &i);
		res = ft_strjoin_free(res, tmp);
		free(tmp);
	}
	return (res);
}
