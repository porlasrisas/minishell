/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 22:52:10 by Guille            #+#    #+#             */
/*   Updated: 2025/08/28 18:30:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>
#include <fcntl.h>

int	open_pipe_if_needed(t_command *cmd, int i, int total, int pipe_fd[2])
{
    (void)cmd;
    pipe_fd[0] = -1;
    pipe_fd[1] = -1;
    if (i < total - 1)
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            return (-1);
        }
        // Sin fcntl: cerraremos FDs extra manualmente en los hijos
    }
    return (0);
}

void	setup_child_pipes(int prev_fd, t_command *cmd, int pipe_fd[2])
{
    (void)cmd;
    if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (pipe_fd[1] != -1)
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    if (pipe_fd[0] != -1)
        close(pipe_fd[0]);
}

void	parent_after_fork(pid_t pid, int *prev_fd, int pipe_fd[2], t_command *cmd)
{
    (void)pid;
    (void)cmd;
    if (*prev_fd != -1)
        close(*prev_fd);
    if (pipe_fd[1] != -1)
    {
        close(pipe_fd[1]);
        *prev_fd = pipe_fd[0];
    }
    else
    {
        *prev_fd = -1;
    }
}

void	finalize_status(t_shell *shell, int last_status)
{
    int					sig;

    if (WIFSIGNALED(last_status))
    {
        sig = WTERMSIG(last_status);
        if (sig == SIGQUIT)
            ft_putstr_fd("Quit: 3\n", 2);
        shell->exit_status = 128 + sig;
    }
    else
        shell->exit_status = WEXITSTATUS(last_status);
}

static int	is_directory(const char *path)
{
    struct stat	st;

    if (!path)
        return (0);
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
        return (1);
    return (0);
}

static void	execve_or_die(char *path, char **argv, char **envp)
{
    // Detectar directorio antes de intentar ejecutar
    if (path && is_directory(path))
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(argv[0], 2);
        ft_putstr_fd(": is a directory\n", 2);
        exit(126);
    }
    execve(path, argv, envp);
    if (errno == ENOENT)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(argv[0], 2);
        if (path && ft_strchr(path, '/'))
            ft_putstr_fd(": No such file or directory\n", 2);
        else
            ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    else if (errno == EACCES)
    {
        // Si es un directorio, dar mensaje específico (fallback por si cambió entre llamadas)
        if (path && is_directory(path))
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(argv[0], 2);
            ft_putstr_fd(": is a directory\n", 2);
            exit(126);
        }
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(argv[0], 2);
        ft_putstr_fd(": Permission denied\n", 2);
        exit(126);
    }
    perror("execve");
    exit(1);
}

static void	close_extra_fds(void)
{
    int	fd;

    fd = 3;
    while (fd < 256)
    {
        close(fd);
        fd++;
    }
}

void	pipe_exec_child(t_shell *shell, t_command *cmd, int prev_fd,
    int pipe_fd[2])
{
    char	*path;

    setup_child_pipes(prev_fd, cmd, pipe_fd);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    if (has_heredoc(cmd))
        handle_redirections_with_heredoc(cmd);
    else
        handle_redirections(cmd);
    // Cerrar cualquier FD extra antes de exec
    close_extra_fds();
    if (!cmd->args || !cmd->args[0] || ft_strlen(cmd->args[0]) == 0)
        exit(1);
    if (ft_is_builtin(cmd->args[0]))
    {
        ft_execute_builtin(shell, cmd);
        exit(shell->exit_status);
    }
    path = cmd->args[0];
    if (ft_strchr(cmd->args[0], '/') == NULL)
    {
        path = ft_resolve_command_path(shell, cmd->args[0]);
        if (!path)
        {
            ft_putstr_fd("minishell: ", 2);
            ft_putstr_fd(cmd->args[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            exit(127);
        }
    }
    execve_or_die(path, cmd->args, shell->env.variables);
}
