/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:49:20 by carbon-m          #+#    #+#             */
/*   Updated: 2025/06/16 19:38:55 by guigonza         ###   ########.fr       */
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