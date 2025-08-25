/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/24 21:28:10 by Guille            #+#    #+#             */
/*   Updated: 2025/08/24 21:00:35 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	escape_replacement(char c)
{
	if (c == 'n')
		return ('\n');
	else if (c == 't')
		return ('\t');
	else if (c == 'r')
		return ('\r');
	else if (c == 'b')
		return ('\b');
	else if (c == 'a')
		return ('\a');
	else if (c == 'v')
		return ('\v');
	else if (c == 'f')
		return ('\f');
	else if (c == '\\')
		return ('\\');
	return (0);
}

static void	process_escape_sequences(char *str)
{
	int	i;
	int	j;
	int	rep;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1])
		{
			i++;
			rep = escape_replacement(str[i]);
			if (rep)
				str[j++] = rep;
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

static void	print_arg(char *arg, int interpret_escapes)
{
	char	*tmp;

	if (interpret_escapes)
	{
		tmp = ft_strdup(arg);
		if (!tmp)
			return ;
		process_escape_sequences(tmp);
		printf("%s", tmp);
		free(tmp);
	}
	else
		printf("%s", arg);
}

static int	parse_echo_flags(char **args, int *print_nl, int *interpret)
{
	int	i;

	i = 1;
	*print_nl = 1;
	*interpret = 0;
	while (args[i] && args[i][0] == '-')
	{
		if (ft_strncmp(args[i], "-n", 2) == 0 && args[i][2] == '\0')
			*print_nl = 0;
		else if (ft_strncmp(args[i], "-e", 2) == 0 && args[i][2] == '\0')
			*interpret = 1;
		else
			break ;
		i++;
	}
	return (i);
}

void	ft_builtin_echo(char **args)
{
	int	i;
	int	print_newline;
	int	interpret_escapes;

	i = parse_echo_flags(args, &print_newline, &interpret_escapes);
	while (args[i])
	{
		print_arg(args[i], interpret_escapes);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (print_newline)
		printf("\n");
}
