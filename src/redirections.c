/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 17:36:30 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redirections(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		handle_single_redirection(&cmd->redirs[i]);
		i++;
	}
}

void	handle_redirections_with_heredoc(t_command *cmd)
{
	int	i;

	i = 0;
	while (i < cmd->redir_count)
	{
		handle_single_redirection(&cmd->redirs[i]);
		i++;
	}
}
