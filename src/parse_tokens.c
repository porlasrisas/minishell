/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:13:51 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 17:16:03 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*ft_new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->args_flagged = NULL;
	cmd->args_count = 0;
	cmd->redirs = NULL;
	cmd->redir_count = 0;
	cmd->pipe_after = 0;
	return (cmd);
}

t_command	*ft_parse_single_cmd(t_shell *shell, char **tokens)
{
	t_command	*cmd;
	int			i;
	int			next_i;

	cmd = ft_new_command();
	if (!cmd)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		next_i = process_token(shell, cmd, tokens, i);
		if (next_i == -1)
			break ;
		i = next_i;
	}
	ft_args_with_flags(cmd);
	return (cmd);
}
