/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/26 18:50:44 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
int	main(int ac, char **av, char **env)
{
    t_shell		shell;
    t_command		**cmds;
    char			*line;

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

    shell.env.count = 0;
    while (env[shell.env.count])
        shell.env.count++;
    shell.env.variables = malloc(sizeof(char *) * (shell.env.count + 1));
    if (!shell.env.variables)
        ft_error("Error al asignar memoria para entorno\n", 1, 2, &shell.free);
    int i = 0;
    while (i < shell.env.count)
    {
        shell.env.variables[i] = ft_strdup(env[i]);
        if (!shell.env.variables[i])
            ft_error("Error al copiar variable de entorno\n", 1, 2, &shell.free);
        i++;
    }
    shell.env.variables[i] = NULL;

	if (ac >= 0)
	{
		while (1)
		{
			setup_shell_signals();
			g_sigint_received = 0;
			line = ft_prompt_line(&shell, "minishell$ ");
			if (!line)
			{
				write(1, "exit\n", 5);
				break ;
			}
			shell.tokens = ft_tokenizer(&shell, line);
			if (!shell.tokens)
			{
				shell.free->ptr = (void *)line;
				ft_error("Error: tokenización fallida\n", 1, 1, &shell.free);
				continue ;
			}
			if (ft_handle_cd(&shell))
			{
				ft_error(NULL, 1, 2, &shell.free);
				continue ;
			}
			cmds = ft_parse_tokens(&shell);
			if (!cmds)
			{
				ft_error("Error en parseo\n", 1, 2, &shell.free);
				continue ;
			}
			executor_run(&shell);
			errno = 0;
			ft_error(NULL, 1, 2, &shell.free);
		}
	}
	return (shell.exit_status);
}