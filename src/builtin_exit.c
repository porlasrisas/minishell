/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 00:06:30 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		printf("exit\n");
	else
		printf("quit\n");
	if (!args[1])
	{
		exit(shell->exit_status);
	}
	if (!ft_is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(args[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		exit(255);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	exit_code = ft_atoi(args[1]);
	if (exit_code < 0)
		exit_code = 256 + (exit_code % 256);
	else if (exit_code > 255)
		exit_code = exit_code % 256;
	exit(exit_code);
}
