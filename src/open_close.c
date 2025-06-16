/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_close.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 12:49:20 by carbon-m          #+#    #+#             */
/*   Updated: 2025/06/16 10:58:46 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_doc(t_shell shell, int num_arg)
{
	int	fd;

	fd = open(shell.commands->redirs->file, O_RDONLY, 644);
	if (fd == -1)
	{
		ft_putstr_fd("zsh: permission denied: ", 2);
		ft_putstr_fd(shell.commands->redirs->file, 2);
	}
	else
		shell.filed.fd[num_arg] = fd;
}

void	create_trunc_doc(t_shell shell, int num_arg)
{
	int	fd;

	fd = open(shell.commands->redirs->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(shell.commands->redirs->file, 2);
		ft_putstr_fd(": Permission denied", 2);
	}
	else
		shell.filed.fd[num_arg] = fd;
}

void	create_append_doc(t_shell shell, int num_arg)
{
	int	fd;

	fd = open(shell.commands->redirs->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(shell.commands->redirs->file, 2);
		ft_putstr_fd(": Permission denied", 2);
	}
	else
		shell.filed.fd[num_arg] = fd;
}
