/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:22:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/02 17:59:11 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_prompt_line(t_shell *shell, const char *prompt)
{
	char	*line;

	line = readline(prompt);
	if (!line)
	{
		printf("exit\n");
		shell->exit_status = 1;
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}
