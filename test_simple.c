/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/21 20:16:03 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
    t_shell shell;
    char *test_line = "echo \"hello world\"";
    
    (void)ac;
    (void)av;
    (void)env;
    
    printf("=== TEST SIMPLE PARSEO ===\n");
    printf("Comando de test: %s\n", test_line);
    
    // Inicialización básica
    shell.exit_status = 0;
    shell.tokens = NULL;
    shell.commands = NULL;
    shell.command_count = 0;
    
    // Test tokenizer
    printf("\n1. Testing tokenizer...\n");
    shell.tokens = ft_tokenizer(&shell, test_line);
    
    if (!shell.tokens)
    {
        printf("ERROR: tokenizer falló\n");
        return 1;
    }
    
    printf("Tokens generados:\n");
    int i = 0;
    while (shell.tokens[i])
    {
        printf("  [%d]: '%s'\n", i, shell.tokens[i]);
        i++;
    }
    printf("Total tokens: %d\n", i);
    
    // Test parser
    printf("\n2. Testing parser...\n");
    shell.commands = ft_parse_tokens(&shell);
    
    if (!shell.commands)
    {
        printf("ERROR: parser falló\n");
        return 1;
    }
    
    printf("Comandos parseados: %d\n", shell.command_count);
    i = 0;
    while (i < shell.command_count && shell.commands[i])
    {
        printf("Comando %d:\n", i);
        if (shell.commands[i]->args)
        {
            int j = 0;
            while (shell.commands[i]->args[j])
            {
                printf("  arg[%d]: '%s'\n", j, shell.commands[i]->args[j]);
                j++;
            }
        }
        printf("  pipe_after: %d\n", shell.commands[i]->pipe_after);
        i++;
    }
    
    printf("\n=== TEST COMPLETADO ===\n");
    
    // Cleanup básico
    if (shell.tokens)
    {
        i = 0;
        while (shell.tokens[i])
        {
            free(shell.tokens[i]);
            i++;
        }
        free(shell.tokens);
    }
    
    return 0;
}
