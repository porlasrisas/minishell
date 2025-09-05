/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:13:51 by guigonza          #+#    #+#             */
/*   Updated: 2025/09/05 14:48:08 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int					grow_redirs(t_redirection **arr, int *count);
char				*process_redir_target(t_shell *shell, char *file,
						t_redir_type type, int *no_expand);

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

static void	ft_add_argument(t_shell *shell, t_command *cmd, char *arg)
{
	size_t	new_size;
	char	**new_array;
	char	*processed_arg;

	processed_arg = ft_process_token_quotes(shell, arg);
	if (!processed_arg || ft_strlen(processed_arg) == 0)
	{
		free(processed_arg);
		return ;
	}
	new_size = (cmd->args_count + 2) * (sizeof(char *));
	new_array = ft_realloc(cmd->args, cmd->args_count * sizeof(char *),
			new_size);
	if (!new_array)
	{
		free(processed_arg);
		return ;
	}
	cmd->args = new_array;
	cmd->args[cmd->args_count] = processed_arg;
	cmd->args[cmd->args_count + 1] = NULL;
	cmd->args_count++;
}

static void	ft_add_redirection(t_shell *shell, t_command *cmd, char *file,
		t_redir_type type)
{
	char	*processed_file;
	int		no_expand;

	if (!grow_redirs(&cmd->redirs, &cmd->redir_count))
		return ;
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].heredoc_content = NULL;
	cmd->redirs[cmd->redir_count].hd_no_expand = 0;
	processed_file = process_redir_target(shell, file, type, &no_expand);
	if (type == REDIR_HEREDOC)
		cmd->redirs[cmd->redir_count].hd_no_expand = no_expand;
	cmd->redirs[cmd->redir_count].file = processed_file;
	cmd->redir_count++;
}

/* helpers moved to parse_redirs_utils.c */

static int	ft_process_token(t_shell *shell, t_command *cmd, char **tokens,
		int *i)
{
	t_redir_type	type;

	type = ft_get_redir_type(tokens[*i]);
	if (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND
		|| type == REDIR_HEREDOC)
	{
		if (!tokens[*i + 1])
			return (0);
		ft_add_redirection(shell, cmd, tokens[*i + 1], type);
		*i += 2;
	}
	else if (type == PIPE || type == SEMICOLON)
		return (0);
	else
	{
		ft_add_argument(shell, cmd, tokens[*i]);
		(*i)++;
	}
	return (1);
}

t_command	*ft_parse_single_cmd(t_shell *shell, char **tokens)
{
	t_command	*cmd;
	int			i;

	cmd = ft_new_command();
	if (!cmd)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		if (!ft_process_token(shell, cmd, tokens, &i))
			break ;
	}
	ft_args_with_flags(cmd);
	return (cmd);
}
