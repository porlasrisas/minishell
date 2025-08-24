/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:47:53 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/18 20:08:12 by carbon           ###   ########.fr       */
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

    // Inicialización de pwd y oldpwd
    shell.env.pwd = getcwd(NULL, 0);
    if (!shell.env.pwd)
        ft_error("Error al obtener el directorio actual\n", 1, 2, &shell.free);
    shell.env.oldpwd = NULL;
    
    // Inicialización del entorno
    // Contar variables de entorno
    shell.env.count = 0;
    while (env[shell.env.count])
        shell.env.count++;
    
    // Crear copia dinámica del entorno
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
    
    printf("DEBUG: Variables de entorno inicializadas: %d\n", shell.env.count);
    
	
	if (ac >= 0)
	{
		while (1)
		{
			setup_shell_signals();
    		g_sigint_received = 0;
			line = ft_prompt_line(&shell, "minishell ->$ ");
            printf("Ruta actual: %s\n", shell.env.pwd);
            printf("Ruta antigua: %s\n", shell.env.oldpwd);
			if (!line)
				break;
			shell.tokens = ft_tokenizer(&shell, line);
			if (!shell.tokens)
			{
				shell.free->ptr = (void *)line;
				ft_error("Error: tokenización fallida\n", 1, 1, &shell.free);
				continue ;
			}
			
			// Debug: mostrar tokens
			printf("=== DEBUG TOKENS ===\n");
			printf("Total tokens: %d\n", shell.token_count);
			shell.i = 0;
			while (shell.tokens[shell.i])
			{
				printf("Token[%d]: '%s'\n", shell.i, shell.tokens[shell.i]);
				shell.i++;
			}
			printf("====================\n");
			
            if (ft_handle_cd(&shell))
            {
                ft_error(NULL, 1, 2, &shell.free);
                continue; // Evitar procesar el comando como otro tipo
            }
			cmds = ft_parse_tokens(&shell);
			if (!cmds)
			{
				ft_error("Error en parseo\n", 1, 2, &shell.free);
				continue;
			}
			printf("Comandos parseados: %d\n", shell.command_count);
			shell.i = 0;
			while (shell.i < shell.command_count)
			{
				printf("=== COMANDO %d ===\n", shell.i);
				if (cmds[shell.i])
				{
					if (cmds[shell.i]->args)
					{
						int j = 0;
						printf("Argumentos:\n");
						while (cmds[shell.i]->args[j])
						{
							printf("  args[%d]: '%s'\n", j, cmds[shell.i]->args[j]);
							j++;
						}
					}
					else
						printf("  args: NULL\n");
					
					if (cmds[shell.i]->args_flagged)
					{
						int j = 0;
						printf("Argumentos con flags:\n");
						while (cmds[shell.i]->args_flagged[j])
						{
							printf("  args_flagged[%d]: '%s'\n", j, cmds[shell.i]->args_flagged[j]);
							j++;
						}
					}
					else
						printf("  args_flagged: NULL\n");
						
					printf("  pipe_after: %d\n", cmds[shell.i]->pipe_after);
				}
				else
					printf("  comando NULL\n");
				printf("===============\n");
				shell.i++;
			}
			
			// EJECUTAR LOS COMANDOS PARSEADOS
			printf("=== EJECUTANDO COMANDOS ===\n");
			if (shell.command_count == 1 && !cmds[0]->pipe_after)
			{
				printf("Ejecutando comando simple...\n");
				ft_execute_simple_command(&shell);
			}
			else if (shell.command_count > 1)
			{
				printf("Ejecutando pipeline...\n");
				ft_execute_pipeline_execve(&shell);
			}
			printf("===========================\n");
			
			printf("history:\n %s\n", line);
			errno = 0; // Limpiar errno antes de llamar ft_error
			ft_error(NULL, 1, 2, &shell.free);
		}
	}
	else
	{
		printf("Usage: ./minishell [options]\n");
	}
	return (shell.exit_status);
}