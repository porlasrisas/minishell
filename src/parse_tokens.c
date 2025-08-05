/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:13:51 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/05 18:24:11 by Guille           ###   ########.fr       */
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
static	void	ft_add_argument(t_shell *shell, t_command	*cmd, char *arg)
{
	size_t	new_size;
	char	**new_array;
	char	*processed_arg;

	printf("DEBUG ft_add_argument: recibido arg='%s'\n", arg ? arg : "NULL");
	new_size = (cmd->args_count + 2) * (sizeof(char *));
	new_array = ft_realloc(cmd->args,cmd->args_count * sizeof(char *), new_size);
	if (!new_array)
	{
		printf("ERROR: ft_realloc falló en ft_add_argument\n");
		return;
	}
	cmd->args = new_array;
	processed_arg = ft_process_token_quotes(shell, arg);
	cmd->args[cmd->args_count] = processed_arg;
	cmd->args[cmd->args_count + 1] = NULL;
	cmd->args_count++;
	printf("DEBUG ft_add_argument: guardado args[%d]='%s'\n", 
		   cmd->args_count - 1, cmd->args[cmd->args_count - 1] ? cmd->args[cmd->args_count - 1] : "NULL");
}

static	void 	ft_add_redirection(t_command *cmd, char *file, t_redir_type	type)
{
	size_t	new_size;
	t_redirection *new_array;
	new_size = (cmd->redir_count + 1)  * (sizeof(t_redirection));
	new_array = ft_realloc(cmd->redirs, cmd->redir_count * sizeof(t_redirection), new_size);
	if (!new_array)
	{
		printf("ERROR: ft_realloc falló en ft_add_redirection\n");
		return;
	}
	cmd->redirs = new_array;
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].heredoc_content = NULL;
	if (type == REDIR_HEREDOC)
	{
		cmd->redirs[cmd->redir_count].file = ft_strdup(file);
		cmd->redirs[cmd->redir_count].heredoc_content = read_heredoc_content(file);
	}
	else if (type != PIPE)
		cmd->redirs[cmd->redir_count].file = ft_strdup(file);
	else
		cmd->redirs[cmd->redir_count].file = ft_strdup("|");
	cmd->redir_count++;
}
t_redir_type	ft_get_redir_type(char *token)
{
	if (ft_strncmp(token, "<<", 2) == 0 && ft_strlen(token) == 2)
		return (REDIR_HEREDOC);
	if (ft_strncmp(token, "<",ft_strlen(token)) == 0)
		return (REDIR_IN);
	else if (ft_strncmp(token, ">>",ft_strlen(token)) == 0)
		return (REDIR_APPEND);
	else if (ft_strncmp(token, ">",ft_strlen(token)) == 0)
		return (REDIR_OUT);
	else if (ft_strncmp(token, "|",ft_strlen(token)) == 0)
		return (PIPE);
	return (REDIR_UNKNOWN);
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
        if (type == REDIR_IN || type == REDIR_OUT || 
            type == REDIR_APPEND || type == REDIR_HEREDOC)
        {
            if (!tokens[i + 1])
            {
                printf("ERROR: redirección sin archivo\n");
                return (NULL);
            }
            ft_add_redirection(cmd, tokens[i + 1], type);
            i += 2;
        }
        else if (type == PIPE)
        {
            break;
        }
        else
        {
            ft_add_argument(shell, cmd, tokens[i]);
            i++;
        }
    }
    
    // Procesar flags después del parseo básico
    ft_args_with_flags(cmd);
    
    return (cmd);
}
