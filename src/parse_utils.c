/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 15:57:50 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_token(const char *s, const char *tok)
{
	return (s && tok && ft_strncmp(s, tok, ft_strlen(tok)) == 0);
}

t_redir_type	ft_get_redir_type(char *token)
{
	if (!token || *token == '\0')
		return (REDIR_UNKNOWN);
	if (is_token(token, "|"))
		return (PIPE);
	if (is_token(token, ";"))
		return (SEMICOLON);
	if (is_token(token, "<<"))
		return (REDIR_HEREDOC);
	if (is_token(token, ">>"))
		return (REDIR_APPEND);
	if (is_token(token, ">"))
		return (REDIR_OUT);
	if (is_token(token, "<"))
		return (REDIR_IN);
	return (REDIR_UNKNOWN);
}

int	ft_count_pipes(t_shell *shell)
{
	int	count;

	shell->i = 0;
	count = 1;
	while (shell->tokens[shell->i])
	{
		if (ft_get_redir_type(shell->tokens[shell->i]) == PIPE ||
			ft_get_redir_type(shell->tokens[shell->i]) == SEMICOLON)
			count++;
		shell->i++;
	}
	return (count);
}

char	**ft_copy_token_segment(t_shell *shell, int start, int end)
{
	int		len;
	char	**segment;

	len = end - start;
	segment = malloc(sizeof(char *) * (len + 1));
	if (!segment)
		return (NULL);
	shell->j = 0;
	while (shell->j < len)
	{
		segment[shell->j] = ft_strdup(shell->tokens[start + shell->j]);
		shell->j++;
	}
	segment[shell->j] = NULL;
	return (segment);
}

int	ft_validate_pipe_syntax(t_shell *shell)
{
	if (!shell->tokens[0])
		return (0);
	if (ft_get_redir_type(shell->tokens[0]) == PIPE || 
		ft_get_redir_type(shell->tokens[0]) == SEMICOLON)
		return (0);
	shell->i = 0;
	while (shell->tokens[shell->i])
	{
		if (ft_get_redir_type(shell->tokens[shell->i]) == PIPE ||
			ft_get_redir_type(shell->tokens[shell->i]) == SEMICOLON)
		{
			if (!shell->tokens[shell->i + 1])
				return (0);
			if (ft_get_redir_type(shell->tokens[shell->i + 1]) == PIPE ||
				ft_get_redir_type(shell->tokens[shell->i + 1]) == SEMICOLON)
				return (0);
		}
		shell->i++;
	}
	return (1);
}

char	*ft_remove_quotes(char *token)
{
	char	*result;
	int		len;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (ft_strdup(token));
	if ((token[0] == '\"' && token[len - 1] == '\"') ||
		(token[0] == '\'' && token[len - 1] == '\''))
	{
		result = ft_substr(token, 1, len - 2);
		return (result);
	}
	return (ft_strdup(token));
}
