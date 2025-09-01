/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 15:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/01 22:50:10 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_line(t_shell *shell, char *line)
{
	if (!line)
	{
		write(1, "exit\n", 5);
		return (0);
	}
	shell->tokens = ft_tokenizer(shell, line);
	if (!shell->tokens)
	{
		shell->free->ptr1 = (void *)line;
		ft_error("Error: tokenización fallida\n", 1, 0, shell->free);
		return (1);
	}
	return (1);
}

static int	handle_commands(t_shell *shell)
{
	t_command	**cmds;

	if (ft_handle_cd(shell))
	{
		ft_error(NULL, 1, 2, shell->free);
		return (1);
	}
	cmds = ft_parse_tokens(shell);
	if (!cmds)
	{
		ft_error("Error en parseo\n", 1, 2, shell->free);
		return (1);
	}
	executor_run(shell);
	errno = 0;
	ft_error(NULL, 1, 2, shell->free);
	return (1);
}

static void	shell_iteration(t_shell *shell)
{
	char	*line;

	setup_shell_signals();
	g_sigint_received = 0;
	line = ft_prompt_line(shell, "minishell ->$ ");
	if (process_line(shell, line))
		handle_commands(shell);
}

void	run_shell_loop(t_shell *shell)
{
	while (1)
	{
		shell_iteration(shell);
		if (!shell->tokens)
			break ;
	}
}

void	init_shell_basic(t_shell *shell)
{
	g_sigint_received = 0;
	shell->exit_status = 0;
	shell->tokens = NULL;
	shell->free = malloc(sizeof(t_format));
	if (!shell->free)
		ft_error("No se pudo asignar memoria\n",
			1, 2, shell->free);
	shell->free->ptr = NULL;
	shell->free->ptr1 = NULL;
	shell->free->depth = 0;
	shell->env.pwd = getcwd(NULL, 0);
	if (!shell->env.pwd)
		ft_error("Error al obtener el directorio actual\n",
			1, 2, shell->free);
	shell->env.oldpwd = NULL;
}
