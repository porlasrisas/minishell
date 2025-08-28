/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 20:40:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/28 18:34:53 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

static int	is_directory(const char *path)
{
    struct stat st;

    if (!path)
        return (0);
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
        return (1);
    return (0);
}

static void	print_err_3(const char *a, const char *b, const char *c)
{
    if (a)
        ft_putstr_fd((char *)a, 2);
    if (b)
        ft_putstr_fd((char *)b, 2);
    if (c)
        ft_putstr_fd((char *)c, 2);
}

static void	exec_external_in_child(t_shell *shell, t_command *cmd)
{
    char	*path;

    path = cmd->args[0];
    if (ft_strchr(cmd->args[0], '/') == NULL)
    {
        path = ft_resolve_command_path(shell, cmd->args[0]);
        if (!path)
        {
            print_err_3("minishell: ", cmd->args[0], ": command not found\n");
            exit(127);
        }
    }
    // Directorio: mismo mensaje que bash
    if (path && is_directory(path))
    {
        print_err_3("minishell: ", cmd->args[0], ": is a directory\n");
        exit(126);
    }
    apply_redirs(cmd);
    setup_child_signals();
    execve(path, cmd->args, shell->env.variables);
    if (errno == ENOENT)
    {
        print_err_3("minishell: ", cmd->args[0], NULL);
        if (path && ft_strchr(path, '/'))
            ft_putstr_fd(": No such file or directory\n", 2);
        else
            ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    if (errno == EACCES)
    {
        // Revalidar por si es directorio
        if (path && is_directory(path))
        {
            print_err_3("minishell: ", cmd->args[0], ": is a directory\n");
            exit(126);
        }
        print_err_3("minishell: ", cmd->args[0], ": Permission denied\n");
        exit(126);
    }
    perror("execve");
    exit(1);
}

static void	execute_external_command(t_shell *shell, t_command *cmd)
{
    pid_t	pid;
    int		status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return ;
    }
    else if (pid == 0)
        exec_external_in_child(shell, cmd);
    else
    {
        waitpid(pid, &status, 0);
        update_status_from_wait(shell, status);
    }
}

static int	exec_builtin_with_redirs(t_shell *shell, t_command *cmd)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (0);
	}
	else if (pid == 0)
	{
		apply_redirs(cmd);
		setup_child_signals();
		ft_execute_builtin(shell, cmd);
		exit(shell->exit_status);
	}
	else
	{
		waitpid(pid, &status, 0);
		update_status_from_wait(shell, status);
	}
	return (1);
}

void	ft_execute_simple_command(t_shell *shell)
{
    t_command	*cmd;

    if (!shell || !shell->commands || !shell->commands[0])
        return ;
    cmd = shell->commands[0];
    if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
    {
        shell->exit_status = 0;
        return ;
    }
    if (ft_is_builtin(cmd->args[0]) && cmd->redir_count == 0)
    {
        ft_execute_builtin(shell, cmd);
        return ;
    }
    if (ft_is_builtin(cmd->args[0]) && cmd->redir_count > 0)
    {
        if (exec_builtin_with_redirs(shell, cmd))
            return ;
    }
    /* Si no hay barra y PATH no contiene el comando, devolver 127 sin forkar */
    if (!ft_strchr(cmd->args[0], '/'))
    {
        char *resolved = ft_resolve_command_path(shell, cmd->args[0]);
        if (!resolved)
        {
            print_err_3("minishell: ", cmd->args[0], ": command not found\n");
            shell->exit_status = 127;
            return ;
        }
        free(resolved);
    }
    execute_external_command(shell, cmd);
}
