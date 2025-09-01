/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 17:22:00 by guigonza          #+#    #+#             */
/*   Updated: 2025/09/01 22:43:03 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

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
	char	*rl_prompt;
	int		interactive;

	(void)prompt;
	interactive = isatty(STDIN_FILENO);
	dynamic_prompt = ft_generate_prompt(shell);
	if (!dynamic_prompt)
		return (NULL);
	if (!interactive)
	{
		write(1, dynamic_prompt, ft_strlen(dynamic_prompt));
		write(1, "\n", 1);
		rl_prompt = "";
	}
	else
		rl_prompt = dynamic_prompt;
	line = readline(rl_prompt);
	free(dynamic_prompt);
	if (!line)
		return (NULL);
	if (interactive && *line)
		add_history(line);
	return (line);
}

char	*ft_remove_quotes(char *token)
{
	char	*result;
	int		len;

	if (!token)
		return (NULL);
	len = ft_strlen(token);
	if (len < 2)
		return (ft_strdup(token));
	if ((token[0] == '"' && token[len - 1] == '"') || (token[0] == '\''
			&& token[len - 1] == '\''))
	{
		result = ft_substr(token, 1, len - 2);
		return (result);
	}
	return (ft_strdup(token));
}
