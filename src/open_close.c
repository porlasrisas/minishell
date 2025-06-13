/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:49:20 by carbon-m          #+#    #+#             */
/*   Updated: 2025/06/13 15:55:23 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_doc(t_shell shell)
{
	int	fd;

	fd = open(shell.commands->redirs->file, O_RDONLY, 644);
	if (fd == -1)
		ft_error("zsh: permission denied: ",1,1);
}