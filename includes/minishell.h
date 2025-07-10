/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/07/10 20:48:19 by carbon           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define TOKENS_LEN 256
# define PATH_MAX 4096
# include "libft/libft.h"
# include <dirent.h>    // opendir, readdir, closedir
# include <fcntl.h>     // open
# include <signal.h>    // signal, sigaction, kill
# include <stdio.h>     // printf, perror
# include <stdlib.h>    // malloc, free, exit, getenv
# include <string.h>    // strerror
# include <sys/ioctl.h> // ioctl
# include <sys/stat.h>  // stat, lstat, fstat
# include <sys/wait.h>  // wait, waitpid, wait3, wait4
# include <term.h>      // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
# include <termios.h>   // tcsetattr, tcgetattr
# include <unistd.h>    // write, access, read, close, fork, getcwd, chdir,
# include <limits.h>
						// unlink, execve, dup, dup2, pipe, isatty, ttyname,
# include <errno.h>
// Incluye las cabeceras de readline para manejar la entrada y el historial
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,

// Tipo de redirección
typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	PIPE,
	REDIR_HEREDOC, // <<
	REDIR_UNKNOWN,
}					t_redir_type;

// Redirección individual
typedef struct s_redirection
{
	char			*file;
	t_redir_type	type;
}					t_redirection;

// Comando individual
typedef struct s_command
{
	char			**args;
	char			**args_flagged;
	int args_count;        // argumentos del comando
	t_redirection *redirs; // array de redirecciones
	int				redir_count;
	int pipe_after; // si este comando tiene un pipe |
}					t_command;

// Historial de comandos ejecutados
typedef struct s_history
{
	char			**commands;
	int				count;
}					t_history;

// Variables de entorno
typedef struct s_env
{
	char			**variables;
	char 			*pwd;
	char			*oldpwd;
	int				count;
}					t_env;

// Estructura general de shell
typedef struct s_shell
{
	char *input_line; // línea cruda leída
	char **tokens;    // tokens divididos
	int				token_count;
	int				i;
	int				j;
	char			*tok;
	t_redirection	*redir;
	t_command *commands; // comandos procesados
	int				command_count;

	t_history history; // historial de comandos
	t_env env;         // entorno
	t_format	*free;

	int exit_status; // último código de salida
}					t_shell;

//// Funciones principales

// prompt
char				*ft_prompt_line(t_shell *shell, const char *prompt);
int	ft_is_metachar(char c);
// tokenizer
char				**ft_tokenizer(t_shell *shell, char *line);
t_redir_type	ft_get_redir_type(char	*token);
t_command *ft_parse_tokens(char **tokens);
char	*ft_get_env(t_env *env, const char *key);
void	ft_update_env(t_env *env);
void	ft_cd(t_shell *shell, char **args);
int	ft_handle_cd(t_shell *shell);
void	tests(t_shell shell, t_command cmd);
int ft_is_flag(char *token);
void ft_args_with_flags(char **tokens, t_command *cmd);




#endif