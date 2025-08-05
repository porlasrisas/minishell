/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/22 16:41:51 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	ft_process_pipe_segment(t_shell *shell, t_command **cmds)
{
	t_format	format;

	shell->segment = ft_copy_token_segment(shell, shell->start_idx, shell->i);
	cmds[shell->cmd_idx] = ft_parse_single_cmd(shell, shell->segment);
	format.ptr = (void **)shell->segment;
	format.ptr1 = NULL;
	format.depth = 0;
	ft_error(NULL, 1, 1, &format);
	if (cmds[shell->cmd_idx])
		cmds[shell->cmd_idx]->pipe_after = 1;
	shell->cmd_idx++;
	shell->start_idx = shell->i + 1;
}

static void	ft_process_final_segment(t_shell *shell, t_command **cmds)
{
	t_format	format;

	shell->segment = ft_copy_token_segment(shell, shell->start_idx, shell->i);
	cmds[shell->cmd_idx] = ft_parse_single_cmd(shell, shell->segment);
	format.ptr = (void **)shell->segment;
	format.ptr1 = NULL;
	format.depth = 0;
	ft_error(NULL, 1, 1, &format);
	if (cmds[shell->cmd_idx])
		cmds[shell->cmd_idx]->pipe_after = 0;
	cmds[++shell->cmd_idx] = NULL;
	shell->command_count = shell->cmd_idx;
}

static t_command	**ft_parse_pipeline(t_shell *shell)
{
	t_command	**cmds;

	if (!ft_validate_pipe_syntax(shell))
		return (NULL);
	shell->num_cmds = ft_count_pipes(shell);
	cmds = malloc(sizeof(t_command *) * (shell->num_cmds + 1));
	if (!cmds)
		return (NULL);
	shell->i = 0;
	shell->start_idx = 0;
	shell->cmd_idx = 0;
	while (shell->tokens[shell->i])
	{
		if (ft_get_redir_type(shell->tokens[shell->i]) == PIPE)
			ft_process_pipe_segment(shell, cmds);
		shell->i++;
	}
	ft_process_final_segment(shell, cmds);
	return (cmds);
}

t_command	**ft_parse_tokens(t_shell *shell)
{
	t_command	**cmds;

	if (!shell || !shell->tokens)
		return (NULL);
	cmds = ft_parse_pipeline(shell);
	if (!cmds)
	{
		ft_error("Error de sintaxis en pipeline\n", 1, 1, NULL);
		return (NULL);
	}
	shell->commands = cmds;
	return (cmds);
}
