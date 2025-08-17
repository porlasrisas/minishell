/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_basic.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 22:30:00 by Guille            #+#    #+#             */
/*   Updated: 2025/07/24 12:55:30 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_builtin_pwd(void)
{
	char	*current_dir;

	current_dir = getcwd(NULL, 0);
	if (current_dir)
	{
		printf("%s\n", current_dir);
		free(current_dir);
	}
	else
		perror("pwd");
}

static void	ft_process_escape_sequences(char *str)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			if (str[i] == 'n')
				str[j++] = '\n';
			else if (str[i] == 't')
				str[j++] = '\t';
			else if (str[i] == 'r')
				str[j++] = '\r';
			else if (str[i] == 'b')
				str[j++] = '\b';
			else if (str[i] == 'a')
				str[j++] = '\a';
			else if (str[i] == 'v')
				str[j++] = '\v';
			else if (str[i] == 'f')
				str[j++] = '\f';
			else if (str[i] == '\\')
				str[j++] = '\\';
			else
			{
				str[j++] = '\\';
				str[j++] = str[i];
			}
		}
		else
			str[j++] = str[i];
		i++;
	}
	str[j] = '\0';
}

void	ft_builtin_echo(char **args)
{
	int	i;
	int	print_newline;
	int	interpret_escapes;
	char	*temp;

	i = 1;
	print_newline = 1;
	interpret_escapes = 0;
	while (args[i] && args[i][0] == '-')
	{
		if (ft_strncmp(args[i], "-n", 2) == 0 && args[i][2] == '\0')
			print_newline = 0;
		else if (ft_strncmp(args[i], "-e", 2) == 0 && args[i][2] == '\0')
			interpret_escapes = 1;
		else
			break;
		i++;
	}
	while (args[i])
	{
		if (interpret_escapes)
		{
			temp = ft_strdup(args[i]);
			ft_process_escape_sequences(temp);
			printf("%s", temp);
			free(temp);
		}
		else
			printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
}
