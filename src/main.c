/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/09/01 22:49:38 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	copy_env_variables(t_shell *shell, char **env)
{
	int		i;

	shell->env.count = 0;
	while (env[shell->env.count])
		shell->env.count++;
	shell->env.variables
		= malloc(sizeof(char *) * (shell->env.count + 1));
	if (!shell->env.variables)
		ft_error("Error al asignar memoria para entorno\n",
			1, 2, shell->free);
	i = 0;
	while (i < shell->env.count)
	{
		shell->env.variables[i] = ft_strdup(env[i]);
		if (!shell->env.variables[i])
			ft_error("Error al copiar variable de entorno\n",
				1, 2, shell->free);
		i++;
	}
	shell->env.variables[i] = NULL;
}

static int	process_tokens_and_cd(t_shell *shell,
			t_command ***cmds_ptr, char *line)
{
	shell->tokens = ft_tokenizer(shell, line);
	if (!shell->tokens)
	{
		shell->free->ptr1 = (void *)line;
		ft_error("Error: tokenización fallida\n",
			1, 0, shell->free);
		return (0);
	}
	if (ft_handle_cd(shell))
	{
		ft_error(NULL, 1, 2, shell->free);
		free_tokens(shell);
		free(line);
		return (0);
	}
	*cmds_ptr = ft_parse_tokens(shell);
	if (!*cmds_ptr)
	{
		ft_error("Error en parseo\n",
			1, 2, shell->free);
		free_tokens(shell);
		free(line);
		return (0);
	}
	return (1);
}

static void	process_command_line(t_shell *shell, t_command **cmds, char *line)
{
	if (!process_tokens_and_cd(shell, &cmds, line))
		return ;
	executor_run(shell);
	errno = 0;
	ft_error(NULL, 1, 2, shell->free);
	free_commands(shell);
	free_tokens(shell);
	free(line);
}

static void	main_shell_loop(t_shell *shell)
{
	t_command	**cmds;
	char		*line;
	int			interactive;

	cmds = NULL;
	line = NULL;
	while (1)
	{
		setup_shell_signals();
		g_sigint_received = 0;
		interactive = isatty(STDIN_FILENO);
		line = ft_prompt_line(shell, "minishell ->$ ");
		if (!line)
		{
			if (interactive)
				write(1, "exit\n", 5);
			break ;
		}
		process_command_line(shell, cmds, line);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	shell;

	(void)av;
	if (ac >= 0)
	{
		init_shell_basic(&shell);
		copy_env_variables(&shell, env);
		main_shell_loop(&shell);
	}
	return (shell.exit_status);
}
