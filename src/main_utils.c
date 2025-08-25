/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 18:45:19 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_copy_env_vars(t_shell *shell, char **env)
{
	int	i;

	i = 0;
	while (i < shell->env.count)
	{
		shell->env.variables[i] = ft_strdup(env[i]);
		if (!shell->env.variables[i])
		{
			ft_error("Error al copiar variable de entorno\n", 1, 2,
				&shell->free);
			return (-1);
		}
		i++;
	}
	shell->env.variables[i] = NULL;
	return (0);
}

int	ft_init_env(t_shell *shell, char **env)
{
	shell->env.count = 0;
	while (env[shell->env.count])
		shell->env.count++;
	shell->env.variables = malloc(sizeof(char *) * (shell->env.count + 1));
	if (!shell->env.variables)
	{
		ft_error("Error al asignar memoria para entorno\n", 1, 2, &shell->free);
		return (-1);
	}
	return (ft_copy_env_vars(shell, env));
}

int	ft_process_line(t_shell *shell, char *line, t_command ***cmds)
{
	shell->tokens = ft_tokenizer(shell, line);
	if (!shell->tokens)
	{
		shell->free->ptr = (void *)line;
		ft_error("Error: tokenización fallida\n", 1, 1, &shell->free);
		return (1);
	}
	if (ft_handle_cd(shell))
	{
		ft_error(NULL, 1, 2, &shell->free);
		return (1);
	}
	*cmds = ft_parse_tokens(shell);
	if (!(*cmds))
	{
		ft_error("Error en parseo\n", 1, 2, &shell->free);
		return (1);
	}
	executor_run(shell);
	errno = 0;
	ft_error(NULL, 1, 2, &shell->free);
	return (0);
}

void	ft_main_loop(t_shell *shell, char **line, t_command ***cmds)
{
	while (1)
	{
		setup_shell_signals();
		g_sigint_received = 0;
		*line = ft_prompt_line(shell, "minishell ->$ ");
		if (!(*line))
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (g_sigint_received)
		{
			shell->exit_status = 130;
			continue ;
		}
		if (**line == '\0')
		{
			free(*line);
			continue ;
		}
		if (ft_process_line(shell, *line, cmds) == 1)
			continue ;
	}
}
