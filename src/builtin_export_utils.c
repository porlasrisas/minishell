/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 00:00:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 16:38:11 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmp_env(const void *a, const void *b)
{
	char *const	*sa = (char *const *)a;
	char *const	*sb = (char *const *)b;

	return (ft_strncmp(*sa, *sb, SIZE_MAX));
}

void	print_export_sorted(t_env *env)
{
	char	**copy;
	int		i;

	copy = malloc(sizeof(char *) * (env->count + 1));
	if (!copy)
		return ;
	i = 0;
	while (i < env->count)
	{
		copy[i] = env->variables[i];
		i++;
	}
	copy[i] = NULL;
	qsort(copy, env->count, sizeof(char *), cmp_env);
	i = 0;
	while (i < env->count)
	{
		ft_print_export_line(copy[i]);
		i++;
	}
	free(copy);
}

void	ft_print_export_line(char *var)
{
	char	*equals_pos;

	printf("declare -x ");
	equals_pos = ft_strchr(var, '=');
	if (equals_pos)
	{
		printf("%.*s=\"%s\"\n", (int)(equals_pos - var), var, equals_pos + 1);
	}
	else
		printf("%s\n", var);
}

void	ft_update_env_var(t_env *env, int index, char *var)
{
	free(env->variables[index]);
	env->variables[index] = ft_strdup(var);
}
