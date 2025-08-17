/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/22 00:26:46 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_builtin_env(t_shell *shell)
{
	int	i;

	ft_putstr_fd("DEBUG env: iniciando con ", 2);
	ft_putnbr_fd(shell->env.count, 2);
	ft_putstr_fd(" variables\n", 2);
	i = 0;
	while (i < shell->env.count)
	{
		ft_putstr_fd(shell->env.variables[i], 1);
		ft_putstr_fd("\n", 1);
		i++;
	}
	ft_putstr_fd("DEBUG env: terminado\n", 2);
}
