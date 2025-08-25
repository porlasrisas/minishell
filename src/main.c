/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 16:13:32 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell		shell;
	t_command	**cmds;
	char		*line;

	g_sigint_received = 0;
	(void)av;
	shell.exit_status = 0;
	shell.tokens = NULL;
	cmds = NULL;
	line = NULL;
	shell.free = malloc(sizeof(t_format));
	if (!shell.free)
		ft_error("No se pudo asignar memoria\n", 1, 2, &shell.free);
	shell.free->ptr = NULL;
	shell.env.pwd = getcwd(NULL, 0);
	if (!shell.env.pwd)
		ft_error("Error al obtener el directorio actual\n", 1, 2, &shell.free);
	shell.env.oldpwd = NULL;
	if (ft_init_env(&shell, env) == -1)
		return (shell.exit_status);
	if (ac >= 0)
	{
		ft_main_loop(&shell, &line, &cmds);
	}
	return (shell.exit_status);
}
