/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/02 16:00:59 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;
	char	*line;
	int		i;

	(void)av;
	(void)env;

	shell.exit_status = 0;
	shell.tokens = NULL;

	line = NULL;

	if (ac >= 0)
	{
		while (1)
		{
			line = ft_prompt_line(&shell, "minishell ->$ ");
			if (!line)
				break;

			shell.tokens = ft_tokenizer(&shell, line);
			if (!shell.tokens)
				ft_error("Error: tokenización fallida\n", 1, 0, line);

			i = 0;
			while (shell.tokens[i] != NULL)
			{
				printf("Token: %s\n", shell.tokens[i]);
				i++;
			}
			printf("history:\n %s\n", line);
		}
	}
	else
	{
		ft_error("Usage: ./minishell [options]\n", 2, 1, shell.tokens, 0, line);
	}
	return (shell.exit_status);
}
