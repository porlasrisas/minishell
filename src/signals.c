/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by Guille            #+#    #+#             */
/*   Updated: 2025/08/25 15:59:44 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

extern char				*rl_line_buffer;
extern int				rl_point;
extern int				rl_end;

volatile sig_atomic_t	g_sigint_received = 0;

void	sigint_handler(int signo)
{
	(void)signo;
	g_sigint_received = 1;
	write(1, "\n", 1);
	rl_on_new_line();
	if (rl_line_buffer)
	{
		rl_line_buffer[0] = '\0';
		rl_point = 0;
		rl_end = 0;
	}
	rl_redisplay();
}

void	setup_shell_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_sigint_handler(int signo)
{
	(void)signo;
	write(1, "\n", 1);
	exit(130);
}

void	setup_heredoc_signals(void)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}