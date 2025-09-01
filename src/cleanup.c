/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 21:18:20 by Guille            #+#    #+#             */
/*   Updated: 2025/09/01 21:53:39 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

static void	free_redirs(t_redirection *redirs, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (redirs[i].file)
			free(redirs[i].file);
		if (redirs[i].heredoc_content)
			free(redirs[i].heredoc_content);
		i++;
	}
	free(redirs);
}

void	free_commands(t_shell *shell)
{
	int	i;

	if (!shell->commands)
		return ;
	i = 0;
	while (shell->commands[i])
	{
		free_args(shell->commands[i]->args);
		free_args(shell->commands[i]->args_flagged);
		if (shell->commands[i]->redirs)
			free_redirs(shell->commands[i]->redirs,
				shell->commands[i]->redir_count);
		free(shell->commands[i]);
		i++;
	}
	free(shell->commands);
	shell->commands = NULL;
	shell->command_count = 0;
}

void	free_tokens(t_shell *shell)
{
	int	i;

	if (!shell->tokens)
		return ;
	i = 0;
	while (shell->tokens[i])
	{
		free(shell->tokens[i]);
		i++;
	}
	free(shell->tokens);
	shell->tokens = NULL;
	shell->token_count = 0;
}
