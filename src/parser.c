/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 17:59:18 by guigonza          #+#    #+#             */
/*   Updated: 2025/07/15 19:44:57 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_metachar(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static char *ft_get_quoted(char *line, int *i, char quote)
{
    int start;
    int len;

    start = *i;
    (*i)++; // Avanzar para ignorar la comilla inicial

    while (line[*i] && line[*i] != quote)
        (*i)++; // Avanzar hasta encontrar la comilla de cierre

    if (line[*i] == quote)
        (*i)++; // Avanzar para ignorar la comilla de cierre

    len = *i - start;
    return (ft_substr(line, start, len));
}

static char	*ft_get_word(char *line, int *i)
{
    int		start;
    char	quote;
    int		tmp;

    start = *i;
    while (line[*i] && !ft_isspace(line[*i]) && !ft_is_metachar(line[*i]))
    {
        if (line[*i] == '\'' || line[*i] == '"')
        {
            quote = line[*i];
            tmp = *i;
            ft_get_quoted(line, &tmp, quote); // Procesar el contenido entre comillas
            *i = tmp; // Actualizar el índice
        }
        else
            (*i)++;
    }
    return (ft_substr(line, start, *i - start));
}

static char	*ft_get_metachar(char *line, int *i)
{
	int	start;

	start = *i;
	if ((line[*i] == '<' && line[*i + 1] == '<') || (line[*i] == '>' && line[*i
			+ 1] == '>') || (line[*i] == '&' && line[*i + 1] == '&')
		|| (line[*i] == '|' && line[*i + 1] == '|'))
		*i += 2;
	else
		(*i)++;
	return (ft_substr(line, start, *i - start));
}

char	**ft_tokenizer(t_shell *shell, char *line)
{
	shell->i = 0;
	shell->j = 0;
	shell->tokens = malloc((MAX_TOKENS + 1) * sizeof(char *));
	if (!shell->tokens)
		return (NULL);
	while (line[shell->i])
	{
		while (line[shell->i] && ft_isspace(line[shell->i]))
			shell->i++;
		if (!line[shell->i])
			break ;
		if (line[shell->i] == '\'' || line[shell->i] == '"')
			shell->tok = ft_get_quoted(line, &shell->i, line[shell->i]);
		else if (ft_is_metachar(line[shell->i]))
			shell->tok = ft_get_metachar(line, &shell->i);
		else
			shell->tok = ft_get_word(line, &shell->i);
		if (shell->tok && shell->j < MAX_TOKENS)
			shell->tokens[shell->j++] = shell->tok;
		else if (shell->tok)
			free(shell->tok);
	}
	shell->tokens[shell->j] = NULL;
	shell->token_count = shell->j;
	return (shell->tokens);
}
