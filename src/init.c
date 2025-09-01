/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 23:10:00 by Guille            #+#    #+#             */
/*   Updated: 2025/09/01 23:09:25 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_shell_basic(t_shell *shell)
{
	g_sigint_received = 0;
	shell->exit_status = 0;
	shell->tokens = NULL;
	shell->free = malloc(sizeof(t_format));
	if (!shell->free)
		ft_error("No se pudo asignar memoria\n", 1, 2, shell->free);
	shell->free->ptr = NULL;
	shell->free->ptr1 = NULL;
	shell->free->depth = 0;
	shell->env.pwd = getcwd(NULL, 0);
	if (!shell->env.pwd)
		ft_error("Error al obtener el directorio actual\n", 1, 2, shell->free);
	shell->env.oldpwd = NULL;
}
