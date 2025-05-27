/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:22:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/05/28 00:44:19 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_prompt_line(t_shell *shell, const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (!line)
	{
		printf("exit\n");
		shell->exit_status = 1;
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}
static void	ft_inizialize_tokens(t_shell	*shell)
{
	int 	i;
	
	i = 0;
	while(i <= MAX_TOKENS)
		shell->tokens[i++] = NULL;
}

char	**ft_tokenizer(t_shell *shell, char *line)
{
	int		i;
	char	*del;
	char	*copy;

	i = 0;
	shell->token_count = 0;
	shell->tokens = malloc((MAX_TOKENS + 1) * sizeof(char *));
	ft_inizialize_tokens(shell);
	if (!shell->tokens)
		return (NULL);
	while (line[i] && ft_isspace(line[i]))
		i++;
	copy = ft_strdup(&line[i]);
	del = strtok(copy, " \t\n");
	while (del && shell->token_count < MAX_TOKENS)
	{
		shell->tokens[shell->token_count] = ft_strdup(del);
		if (!shell->tokens[shell->token_count])
			ft_error("Error al tokenizar: ", 1, shell->tokens, copy);
		shell->token_count++;
		del = strtok(NULL, " \t\n");
	}
	shell->tokens[shell->token_count] = NULL;
	free(copy);
	return (shell->tokens);
}
