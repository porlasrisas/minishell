/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/06/02 20:08:40 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define TOKENS_LEN 256
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
	//unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot

// Incluye las cabeceras de readline para manejar la entrada y el historial
# include <readline/history.h>  // add_history, rl_clear_history
# include <readline/readline.h> // readline, rl_on_new_line, rl_replace_line,

// Tipo de redirección
typedef enum e_redir_type
{
	REDIR_IN,     // <
	REDIR_OUT,    // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
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
	char **args;           // argumentos del comando
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
	t_command *commands; // comandos procesados
	int				command_count;

	t_history history; // historial de comandos
	t_env env;         // entorno

	int exit_status; // último código de salida
}					t_shell;

//// Funciones principales

// prompt
char				*ft_prompt_line(t_shell *shell, const char *prompt);

// tokenizer
char				**ft_tokenizer(t_shell *shell, char *line);

#endif