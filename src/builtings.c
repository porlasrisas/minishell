/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtings.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 21:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/21 22:05:24 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "cd", 2) == 0 && (cmd[2] == '\0' || cmd[2] == ' '))
		return (1);
	if (ft_strncmp(cmd, "echo", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (1);
	if (ft_strncmp(cmd, "pwd", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (1);
	if (ft_strncmp(cmd, "export", 6) == 0 && (cmd[6] == '\0' || cmd[6] == ' '))
		return (1);
	if (ft_strncmp(cmd, "unset", 5) == 0 && (cmd[5] == '\0' || cmd[5] == ' '))
		return (1);
	if (ft_strncmp(cmd, "env", 3) == 0 && (cmd[3] == '\0' || cmd[3] == ' '))
		return (1);
	if (ft_strncmp(cmd, "exit", 4) == 0 && (cmd[4] == '\0' || cmd[4] == ' '))
		return (1);
	return (0);
}

void	ft_builtin_pwd(void)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (current_dir)
	{
		printf("%s\n", current_dir);
		free(current_dir);
	}
	else
		perror("pwd");
}

void	ft_builtin_echo(char **args)
{
	int	i;
	int	print_newline;

	i = 1;
	print_newline = 1;
	if (args[1] && ft_strncmp(args[1], "-n", 2) == 0)
	{
		print_newline = 0;
		i = 2;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
}

void	ft_builtin_env(t_shell *shell)
{
	int	i;

	i = 0;
	while (i < shell->env.count)
	{
		printf("%s\n", shell->env.variables[i]);
		i++;
	}
}

void	ft_builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	exit_code = 0;
	if (args[1])
	{
		exit_code = ft_atoi(args[1]);
		if (exit_code < 0 || exit_code > 255)
			exit_code = exit_code % 256;
	}
	printf("exit\n");
	shell->exit_status = exit_code;
	exit(exit_code);
}

void	ft_execute_builtin(t_shell *shell, t_command *cmd)
{
	printf("DEBUG: Ejecutando builtin '%s'\n", cmd->args[0]);
	
	if (ft_strncmp(cmd->args[0], "cd", 2) == 0 && (cmd->args[0][2] == '\0' || cmd->args[0][2] == ' '))
	{
		ft_cd(shell, cmd->args);
		return;
	}
	if (ft_strncmp(cmd->args[0], "pwd", 3) == 0 && (cmd->args[0][3] == '\0' || cmd->args[0][3] == ' '))
	{
		ft_builtin_pwd();
		return;
	}
	if (ft_strncmp(cmd->args[0], "echo", 4) == 0 && (cmd->args[0][4] == '\0' || cmd->args[0][4] == ' '))
	{
		ft_builtin_echo(cmd->args);
		return;
	}
	if (ft_strncmp(cmd->args[0], "env", 3) == 0 && (cmd->args[0][3] == '\0' || cmd->args[0][3] == ' '))
	{
		ft_builtin_env(shell);
		return;
	}
	if (ft_strncmp(cmd->args[0], "exit", 4) == 0 && (cmd->args[0][4] == '\0' || cmd->args[0][4] == ' '))
	{
		ft_builtin_exit(shell, cmd->args);
		return;
	}
	
	printf("DEBUG: Builtin '%s' no implementado aún\n", cmd->args[0]);
}

void	ft_execute_simple_command(t_shell *shell)
{
	t_command	*cmd;
	char		*path;
	pid_t		pid;
	int			status;

	if (!shell || !shell->commands || !shell->commands[0])
		return;
		
	cmd = shell->commands[0];
	if (!cmd->args || !cmd->args[0])
	{
		printf("DEBUG: Comando vacío, ignorando\n");
		return;
	}
	
	printf("DEBUG: Ejecutando comando simple '%s'\n", cmd->args[0]);
	
	// Verificar si es un builtin
	if (ft_is_builtin(cmd->args[0]))
	{
		ft_execute_builtin(shell, cmd);
		return;
	}
	
	// Si no es builtin, ejecutar como comando externo
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return;
	}
	else if (pid == 0)
	{
		// Proceso hijo
		path = cmd->args[0];
		if (ft_strchr(cmd->args[0], '/') == NULL)
		{
			path = ft_resolve_command_path(shell, cmd->args[0]);
			if (!path)
			{
				printf("DEBUG: Comando '%s' no encontrado\n", cmd->args[0]);
				exit(127);
			}
		}
		
		handle_redirections(cmd);
		execve(path, cmd->args, shell->env.variables);
		perror("execve failed");
		exit(1);
	}
	else
	{
		// Proceso padre
		waitpid(pid, &status, 0);
		shell->exit_status = WEXITSTATUS(status);
	}
}

