/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guigonza <guigonza@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/05/27 22:52:17 by guigonza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
	#define MINISHELL_H

#define MAX_TOKENS 100
#define TOKENS_LEN 256
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // malloc, free, exit, getenv
#include <unistd.h>     // write, access, read, close, fork, getcwd, chdir, unlink, execve, dup, dup2, pipe, isatty, ttyname, ttyslot
#include <fcntl.h>      // open
#include <sys/wait.h>   // wait, waitpid, wait3, wait4
#include <signal.h>     // signal, sigaction, kill
#include <sys/stat.h>   // stat, lstat, fstat
#include <dirent.h>     // opendir, readdir, closedir
#include <string.h>     // strerror
#include <sys/ioctl.h>  // ioctl
#include <termios.h>    // tcsetattr, tcgetattr
#include <term.h>       // tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
#include "libft/libft.h"

// Incluye las cabeceras de readline para manejar la entrada y el historial
#include <readline/readline.h>  // readline, rl_on_new_line, rl_replace_line, rl_redisplay
#include <readline/history.h>   // add_history, rl_clear_history



typedef struct s_command {
    char **args;
    char *input_redir;
    char *output_redir;
    int pipe;
} t_command;
typedef struct s_history {
    char **commands;
    int count;
} t_history;
typedef struct s_env {
    char **variables;
} t_env;
typedef struct s_redirection {
    char *file;
    int type; // 0: input, 1: output, 2: append, etc.
} t_redirection;
typedef struct s_pipeline {
    t_command **commands;
    int num_commands;
} t_pipeline;
typedef struct s_operator {
    int type; // 0: AND, 1: OR
    t_command *left;
    t_command *right;
} t_operator;

/* typedef struct s_wildcard {
    char **files;
    int count;
} t_wildcard; */
/* Descripción: Gestiona la expansión de wildcards (*) en el directorio actual.
Importancia: Es útil para funcionalidades avanzadas, pero no es obligatoria. */

typedef struct s_shell {
    t_history *history;
    t_env *env;
    t_command *command;
	t_redirection	*redirections;
	t_pipeline		*pipes;
	char			**tokens;
	int				token_count;
	
    int exit_status;
} t_shell;
char	*ft_prompt_line(t_shell *shell, const char *prompt);
char **ft_tokenizer(t_shell	*shell, char *line);


#endif