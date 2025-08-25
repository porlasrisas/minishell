/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:22:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 17:41:07 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_get_env(t_env *env, const char *key)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (i < env->count)
	{
		if (ft_strncmp(env->variables[i], key, key_len) == 0
			&& env->variables[i][key_len] == '=')
			return (env->variables[i] + (key_len + 1));
		i++;
	}
	return (NULL);
}

void	ft_update_env(t_env *env)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		perror("cd");
		return ;
	}
	if (env->pwd)
	{
		if (env->oldpwd)
			free(env->oldpwd);
		env->oldpwd = env->pwd;
	}
	else
	{
		if (env->oldpwd)
			free(env->oldpwd);
		env->oldpwd = NULL;
	}
	env->pwd = new_pwd;
}

static char	*ft_generate_prompt(t_shell *shell)
{
	char	*cwd;
	char	*prompt;
	char	*temp;

	(void)shell;
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ft_strdup("minishell ->$ "));
	temp = ft_strjoin("minishell:", cwd);
	free(cwd);
	if (!temp)
		return (ft_strdup("minishell ->$ "));
	prompt = ft_strjoin(temp, "/ ->$ ");
	free(temp);
	if (!prompt)
		return (ft_strdup("minishell ->$ "));
	return (prompt);
}

char	*ft_prompt_line(t_shell *shell, const char *prompt)
{
	char	*line;
	char	*dynamic_prompt;

	(void)prompt;
	dynamic_prompt = ft_generate_prompt(shell);
	line = readline(dynamic_prompt);
	free(dynamic_prompt);
	if (!line)
	{
		shell->exit_status = 1;
		return (NULL);
	}
	if (*line)
		add_history(line);
	return (line);
}
