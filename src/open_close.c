/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:49:20 by carbon-m          #+#    #+#             */
/*   Updated: 2025/09/01 13:32:52 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_doc(t_shell shell)
{
	int	fd;

	if (!shell.commands || !shell.commands[0] || !shell.commands[0]->redirs)
		return ;
	fd = open(shell.commands[0]->redirs->file, O_RDONLY, 644);
	if (fd == -1)
		perror("zsh: permission denied");
}
