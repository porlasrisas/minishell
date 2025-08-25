/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_expansion_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 18:30:00 by Guille           #+#    #+#             */
/*   Updated: 2025/08/25 18:30:00 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_double_quotes(t_shell *shell, char *token, int len)
{
	char	*content;
	char	*result;

	content = ft_substr(token, 1, len - 2);
	result = ft_expand_double_quotes(shell, content);
	free(content);
	return (result);
}

static char	*handle_single_quotes(char *token, int len)
{
	return (ft_substr(token, 1, len - 2));
}

static char	*handle_unquoted_with_vars(t_shell *shell, char *token)
{
	return (ft_expand_unquoted_token(shell, token));
}

char	*process_token_by_type(t_shell *shell, char *token)
{
	int	len;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
		return (handle_double_quotes(shell, token, len));
	if (len >= 2 && token[0] == '\'' && token[len - 1] == '\'')
		return (handle_single_quotes(token, len));
	if (ft_strchr(token, '$'))
		return (handle_unquoted_with_vars(shell, token));
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
