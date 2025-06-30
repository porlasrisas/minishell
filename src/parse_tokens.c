/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:13:51 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/30 17:39:05 by carbon           ###   ########.fr       */
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
	cmd->args_count = 0;
	cmd->redirs = NULL;
	cmd->redir_count = 0;
	cmd->pipe_after = 0;
	return (cmd);
}
static	void	ft_add_argument(t_command	*cmd, char *arg)
{
	size_t	new_size;
	char	**new_array;
	new_size = (cmd->args_count + 2) * (sizeof(char *));
	new_array = ft_realloc(cmd->args,cmd->args_count * sizeof(char *), new_size);
	if (!new_array)
		ft_error("Error al añadir argumento: \n",1, 1, new_array);
	cmd->args = new_array;
	cmd->args[cmd->args_count] = arg; 
	cmd->args[cmd->args_count + 1] = NULL;
	cmd->args_count++;
}

static	void 	ft_add_redirection(t_command *cmd, char *file, t_redir_type	type)
{
	size_t	new_size;
	t_redirection *new_array;
	new_size = (cmd->redir_count + 1)  * (sizeof(t_redirection));
	new_array = ft_realloc(cmd->redirs, cmd->redir_count * sizeof(t_redirection), new_size);
	if (!new_array)
		ft_error("Error al añadir redirección: \n", 1, 1, new_array);
	cmd->redirs = new_array;
	cmd->redirs[cmd->redir_count].type = type;
	cmd->redirs[cmd->redir_count].file = file;
	cmd->redir_count++;
}
t_redir_type	ft_get_redir_type(char *token)
{
	if (ft_strncmp(token, "<",ft_strlen(token)) == 0)
		return (REDIR_IN);
	else if (ft_strncmp(token, ">",ft_strlen(token)) == 0)
		return (REDIR_OUT);
	else if (ft_strncmp(token, ">>",ft_strlen(token)) == 0)
		return (REDIR_APPEND);
	else if (ft_strncmp(token, "|",ft_strlen(token)) == 0)
		return (PIPE);
	return (REDIR_UNKNOWN);
}

t_command	*ft_parse_tokens(char **tokens)
{
	t_command	*cmd;
	t_redir_type	type;
	size_t	i;

	i = 0;
	cmd = ft_new_command();
	if (!cmd)
		return (NULL);
	while(tokens[i])
	{
		printf("Token para procesar %ld: %s\n",i, tokens[i]);
		if (ft_is_metachar(tokens[i][0]))
		{
			printf("Token a meta de token %ld: %s\n",i, tokens[i]);
			type = ft_get_redir_type(tokens[i]);
			if (!tokens[i + 1])
			{
				ft_error("Error: no hay archivo para la redirección.\n", 1, 1, NULL);
				break ;
			}
			ft_add_redirection(cmd, tokens[i + 1], type);
			++i;
		}
		else
		{
			printf("Token a comando de token %ld: %s\n",i, tokens[i]);
			ft_add_argument(cmd, tokens[i]);
		}
		i++;
	}
	return (cmd);
}
