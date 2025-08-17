/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 17:01:18 by carbon            #+#    #+#             */
/*   Updated: 2025/08/17 14:05:59 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
#include <stdio.h>

void	sigint_handler(int signo)
{
    (void)signo;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
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
    exit(130); // Bash returns 130 for SIGINT
}

void	setup_heredoc_signals(void)
{
    signal(SIGINT, heredoc_sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}