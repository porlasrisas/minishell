/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/03 19:30:44 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;
	t_command	*cmd;
	char	*line;
	int		i;

	(void)av;
	(void)env;

	shell.exit_status = 0;
	shell.tokens = NULL;
	cmd = NULL;
	line = NULL;
	shell.free = malloc(sizeof(t_format));
	if (!shell.free)
		ft_error("No se pudo asignar memoria\n", 1, 2, &shell.free);
	shell.free->ptr = NULL;
	if (ac >= 0)
	{
		while (1)
		{
			line = ft_prompt_line(&shell, "minishell ->$ ");
			if (!line)
				break;
			shell.tokens = ft_tokenizer(&shell, line);
			if (!shell.tokens)
			{
				shell.free->ptr = (void *)line;
				ft_error("Error: tokenización fallida\n", 1, 1, &shell.free);
				continue ;
			}
			cmd = ft_parse_tokens(shell.tokens);
			i = 0;
			while (shell.tokens[i] != NULL)
			{
				printf("Token: %s\n", shell.tokens[i]);
				i++;
			}
			i = 0;
			while (cmd && cmd->args && cmd->args[i] != NULL)
			{
				printf("Argumentos: %s\n", cmd->args[i]);
				i++;
			}
			printf("history:\n %s\n", line);
			ft_error(NULL, 1, 2, &shell.free);
		}
	}
	else
	{
		ft_error("Usage: ./minishell [options]\n", 2, 1, shell.tokens, 0, line);
	}
	return (shell.exit_status);
}