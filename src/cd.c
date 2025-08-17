/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 15:37:24 by carbon-m          #+#    #+#             */
/*   Updated: 2025/07/21 21:12:54 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void ft_cd_error(const char *path)
{
    const char *error_msg;

    if (!path)
        error_msg = "cd: HOME not set\n";
    else if (errno == ENOENT)
        error_msg = ": No such file or directory\n";
    else if (errno == EACCES)
        error_msg = ": Permission denied\n";
    else if (errno == ENOTDIR)
        error_msg = ": Not a directory\n";
    else
        error_msg = ": Error\n";

    ft_putstr_fd("cd: ", 2);
    if (path)
        ft_putstr_fd((char *)path, 2);
    ft_putstr_fd((char *)error_msg, 2);
}

int	ft_handle_cd(t_shell *shell)
{
    if (shell->tokens && shell->tokens[0] && shell->tokens[0][0] != '\0' &&
        ft_strncmp(shell->tokens[0], "cd", 2) == 0 && 
        (shell->tokens[0][2] == '\0' || shell->tokens[0][2] == ' '))
    {
        printf("DEBUG: Comando CD detectado\n");
        ft_cd(shell, shell->tokens);
        return (1); // Indica que el comando fue manejado
    }
    return (0); // No es un comando `cd`
}

void	ft_cd(t_shell *shell, char **args)
{
    char	*new_pwd;

    printf("DEBUG ft_cd: Ejecutando CD con arg='%s'\n", args[1] ? args[1] : "HOME");
    
    if (!args[1]) // Si no se pasa un argumento, ir al directorio HOME
    {
        char *home = getenv("HOME");
        if (!home || chdir(home) != 0)
        {
            ft_cd_error(home);
            return;
        }
    }
    else if (chdir(args[1]) != 0) // Cambiar al directorio especificado
    {
        ft_cd_error(args[1]);
        return;
    }

    // Actualizar oldpwd
    if (shell->env.oldpwd)
        free(shell->env.oldpwd);
    shell->env.oldpwd = shell->env.pwd;

    // Actualizar pwd
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        ft_cd_error(NULL);
        return;
    }
    shell->env.pwd = new_pwd;
    printf("DEBUG ft_cd: CD exitoso. Nueva ruta: %s\n", shell->env.pwd);
}