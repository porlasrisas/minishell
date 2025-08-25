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

void	ft_builtin_exit(t_shell *shell, char **args)
{
	int	exit_code;

	exit_code = shell->exit_status;
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
