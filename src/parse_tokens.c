/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:13:51 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/27 17:59:43 by Guille           ###   ########.fr       */
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
static void	ft_add_argument(t_shell *shell, t_command *cmd, char *arg)
{
	size_t	new_size;
	char	**new_array;
	char	*processed_arg;

	processed_arg = ft_process_token_quotes(shell, arg);
	if (!processed_arg || ft_strlen(processed_arg) == 0)
	{
		free(processed_arg);
		return;  // No añadir argumentos vacíos
	}
	new_size = (cmd->args_count + 2) * (sizeof(char *));
	new_array = ft_realloc(cmd->args, cmd->args_count * sizeof(char *), new_size);
	if (!new_array)
	{
		free(processed_arg);
		return;
	}
	cmd->args = new_array;
	cmd->args[cmd->args_count] = processed_arg;
	cmd->args[cmd->args_count + 1] = NULL;
	cmd->args_count++;
}

static void	ft_add_redirection(t_shell *shell, t_command *cmd, char *file, t_redir_type type)
{
	size_t			new_size;
	t_redirection	*new_array;
	char			*processed_file;

	new_size = (cmd->redir_count + 1) * sizeof(t_redirection);
	new_array = ft_realloc(cmd->redirs, cmd->redir_count * sizeof(t_redirection), new_size);
	if (!new_array)
		return;
	cmd->redirs = new_array;
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].heredoc_content = NULL;
	
	// Para heredoc, procesar comillas del delimitador
	if (type == REDIR_HEREDOC)
		processed_file = ft_remove_quotes(file);
	else
		processed_file = ft_process_token_quotes(shell, file);
		
	cmd->redirs[cmd->redir_count].file = processed_file;
	cmd->redir_count++;
}

 t_command	*ft_parse_single_cmd(t_shell *shell, char **tokens)
{
    t_command	*cmd;
    int			i;
    t_redir_type	type;

    cmd = ft_new_command();
    if (!cmd)
        return (NULL);
    i = 0;
    while (tokens[i])
    {
        type = ft_get_redir_type(tokens[i]);
        if (type == REDIR_IN || type == REDIR_OUT || type == REDIR_APPEND || type == REDIR_HEREDOC)
        {
            if (!tokens[i + 1])
                return (NULL);
            // Para heredoc, no expandas aquí; solo guarda el delimitador literal
            ft_add_redirection(shell, cmd, tokens[i + 1], type);
            i += 2;
        }
        else if (type == PIPE || type == SEMICOLON)
            break;
        else
        {
            ft_add_argument(shell, cmd, tokens[i]);
            i++;
        }
    }
    ft_args_with_flags(cmd);
    return (cmd);
}
