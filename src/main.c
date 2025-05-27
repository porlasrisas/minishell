/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/05/28 00:47:25 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char *line;
	int j;
	t_shell shell;

	shell.history = NULL;
	shell.env = NULL;
	shell.exit_status = 0;
	j = 1;
	int i = 0;
	env = NULL;
	av = NULL;
	if (ac >= 0)
	{
		while (1)
		{
			line = ft_prompt_line(&shell, "minishell>$ ");
			if (!line)
				break ;
			if (shell.tokens)
				shell.tokens = ft_tokenizer(&shell, line);
			while (shell.tokens[i] != NULL)
			{
				printf("Token: %s\n", shell.tokens[i]);
				i++;
			}
			printf("history:\n %s\n", line);
			free(line);
		}
	}
	else
	{
		ft_putstr_fd("Usage: ./minishell [options]\n", 2);
		return (1);
	}
	return (shell.exit_status);
}