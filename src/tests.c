/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:23:40 by carbon            #+#    #+#             */
/*   Updated: 2025/07/21 10:14:22 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	tests(t_shell shell, t_command cmd)
{
	int i, j;

	i = 0;
	j = 0;
	printf("Valores en t_Shell: \n");
	while (shell.tokens[i])
	{
		printf("Token %d: %s\n", i + 1, shell.tokens[i]);
		++i;
	}
	printf("Numero de comandos: %d\n", cmd.args_count);
	while (cmd.args[j])
	{
		printf("Command %d: %s\n", j + 1, cmd.args[j]);
		++j;
	}
	i = 0;
	printf("Num of redirs: %d\n", cmd.redir_count);
	if (cmd.redir_count != 0)
	{
		i = cmd.redir_count - 1;
		j = 0;
		while (j <= i)
		{
			printf("Redir file %d: %s\n", j, cmd.redirs[j].file);
			printf("Redir type : %d\n", cmd.redirs[j].type);
			++j;
		}
		
	}
	i = 0;
	while (cmd.args_flagged[i])
	{
		printf("Args con flag: %s\n", cmd.args_flagged[i]);
		++i;
	}
}