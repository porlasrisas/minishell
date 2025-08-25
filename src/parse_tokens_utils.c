/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 17:17:56 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_argument(t_shell *shell, t_command *cmd, char *arg)
{
	size_t	new_size;
	char	**new_array;
	char	*processed_arg;

	new_size = (cmd->args_count + 2) * (sizeof(char *));
	new_array = ft_realloc(cmd->args, cmd->args_count * sizeof(char *),
			new_size);
	if (!new_array)
		return ;
	cmd->args = new_array;
	processed_arg = ft_process_token_quotes(shell, arg);
	cmd->args[cmd->args_count] = processed_arg;
	cmd->args[cmd->args_count + 1] = NULL;
	cmd->args_count++;
}

void	ft_add_redirection(t_command *cmd, char *file, t_redir_type type)
{
	size_t			new_size;
	t_redirection	*new_array;
	char			*processed_file;

	new_size = (cmd->redir_count + 1) * sizeof(t_redirection);
	new_array = ft_realloc(cmd->redirs, cmd->redir_count
			* sizeof(t_redirection), new_size);
	if (!new_array)
		return ;
	cmd->redirs = new_array;
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].heredoc_content = NULL;
	if (type == REDIR_HEREDOC)
		processed_file = ft_remove_quotes(file);
	else
		processed_file = ft_strdup(file);
	cmd->redirs[cmd->redir_count].file = processed_file;
	cmd->redir_count++;
}

int	process_redirection_token(t_command *cmd, char **tokens, int i,
		t_redir_type type)
{
	if (!tokens[i + 1])
		return (-1);
	ft_add_redirection(cmd, tokens[i + 1], type);
	return (i + 2);
}

int	process_token(t_shell *shell, t_command *cmd, char **tokens, int i)
{
	t_redir_type	type;

	type = ft_get_redir_type(tokens[i]);
	if (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == REDIR_HEREDOC)
		return (process_redirection_token(cmd, tokens, i, type));
	else if (type == PIPE || type == SEMICOLON)
		return (-1);
	else
	{
		ft_add_argument(shell, cmd, tokens[i]);
		return (i + 1);
	}
}
