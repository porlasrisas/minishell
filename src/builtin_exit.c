/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+      */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/02 15:28:40 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
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

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count])
		count++;
	return (count);
}

static void	print_exit_message(void)
{
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 1);
}

static void	handle_exit_error(t_shell *shell, char *arg, int is_numeric_error)
{
	if (is_numeric_error)
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		shell->exit_status = 255;
		exit(255);
	}
	else
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->exit_status = 1;
	}
}

void	ft_builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;
	int	arg_count;

	exit_code = shell->exit_status;
	arg_count = count_args(args);
	print_exit_message();
	if (arg_count > 1)
	{
		if (!is_numeric(args[1]))
			handle_exit_error(shell, args[1], 1);
		if (arg_count > 2)
		{
			handle_exit_error(shell, NULL, 0);
			return ;
		}
		exit_code = ft_atoi(args[1]);
		if (exit_code < 0 || exit_code > 255)
			exit_code = exit_code % 256;
	}
	shell->exit_status = exit_code;
	exit(exit_code);
}
