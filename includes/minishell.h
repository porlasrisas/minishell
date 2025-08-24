/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon <carbon@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/18 17:10:32 by carbon           ###   ########.fr       */
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

//variable global para las señales
extern volatile sig_atomic_t g_sigint_received;

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
	char			*heredoc_content;
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
	t_command **commands; // array de comandos procesados (para pipelines)
	int				command_count;
	
	// Variables para gestión de pipes
	int				start_idx;     // índice de inicio del comando actual
	int				cmd_idx;       // índice del comando en el array
	int				num_cmds;      // número total de comandos en pipeline
	char			**segment;     // segmento temporal de tokens

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
t_command	**ft_parse_tokens(t_shell *shell);
t_command	*ft_parse_single_cmd(t_shell *shell, char **tokens);

// parse utils
int				ft_count_pipes(t_shell *shell);
char			**ft_copy_token_segment(t_shell *shell, int start, int end);
int				ft_validate_pipe_syntax(t_shell *shell);
char	*ft_remove_quotes(char *token);

char	*ft_get_env(t_env *env, const char *key);
void	ft_update_env(t_env *env);
void	ft_cd(t_shell *shell, char **args);
int		ft_handle_cd(t_shell *shell);
void	tests(t_shell shell, t_command cmd);
int 	ft_is_flag(char *token);
void 	ft_args_with_flags(t_command *cmd);
int		ft_is_builtin(char *cmd);
void	ft_builtin_pwd(void);
void	ft_builtin_echo(char **args);
void	ft_builtin_env(t_shell *shell);
void	ft_builtin_export(t_shell *shell, char **args);
void	ft_builtin_unset(t_shell *shell, char **args);
void	ft_builtin_exit(t_shell *shell, char **args);
void	ft_execute_builtin(t_shell *shell, t_command *cmd);
void	ft_execute_simple_command(t_shell *shell);
void	ft_execute_pipeline_execve(t_shell *shell);
char	*ft_resolve_command_path(t_shell *shell, char *cmd);
void	handle_redirections(t_command *cmd);

// Quote and variable expansion functions
char	*ft_expand_variable(t_shell *shell, char *var_name);
char	*ft_expand_double_quotes(t_shell *shell, char *str);
char	*ft_expand_unquoted_token(t_shell *shell, char *token);
char	*ft_process_token_quotes(t_shell *shell, char *token);
int		ft_is_redirection_token(char *token);

// Heredoc functions from Carbon
void	handle_redirections_with_heredoc(t_command *cmd);
void	handle_heredoc(const char *delimiter);
char 	*read_heredoc_content(const char *delimiter);
int		has_heredoc(t_command *cmd);

// Signal handling
void	setup_shell_signals(void);
void	sigint_handler(int signo);
void	heredoc_sigint_handler(int signo);
void	setup_heredoc_signals(void);

#endif