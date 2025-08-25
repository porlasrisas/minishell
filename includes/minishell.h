/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Guille <Guille@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 12:50:40 by guigonza          #+#    #+#             */
/*   Updated: 2025/08/25 17:53:24 by Guille           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define MAX_TOKENS 100
# define TOKENS_LEN 256
# define PATH_MAX 4096

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_sigint_received;

typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	PIPE,
	REDIR_HEREDOC,
	SEMICOLON,
	REDIR_UNKNOWN
}								t_redir_type;

typedef struct s_redirection
{
	char						*file;
	char						*heredoc_content;
	t_redir_type				type;
}								t_redirection;

typedef struct s_command
{
	char						**args;
	char						**args_flagged;
	int							args_count;
	t_redirection				*redirs;
	int							redir_count;
	int							pipe_after;
}								t_command;

typedef struct s_history
{
	char						**commands;
	int							count;
}								t_history;

typedef struct s_env
{
	char						**variables;
	char						*pwd;
	char						*oldpwd;
	int							count;
}								t_env;

typedef struct s_shell
{
	char						*input_line;
	char						**tokens;
	int							token_count;
	int							i;
	int							j;
	char						*tok;
	t_redirection				*redir;
	t_command					**commands;
	int							command_count;
	int							start_idx;
	int							cmd_idx;
	int							num_cmds;
	char						**segment;
	t_history					history;
	t_env						env;
	t_format					*free;
	int							exit_status;
}								t_shell;

char							*ft_prompt_line(t_shell *shell,
									const char *prompt);
int								ft_is_metachar(char c);
char							**ft_tokenizer(t_shell *shell, char *line);
t_redir_type					ft_get_redir_type(char *token);
t_command						**ft_parse_tokens(t_shell *shell);
t_command						*ft_parse_single_cmd(t_shell *shell,
									char **tokens);
void							ft_add_argument(t_shell *shell, t_command *cmd,
									char *arg);
void							ft_add_redirection(t_command *cmd, char *file,
									t_redir_type type);
int								process_redirection_token(t_command *cmd,
									char **tokens, int i, t_redir_type type);
int								process_token(t_shell *shell, t_command *cmd,
									char **tokens, int i);
int								ft_count_pipes(t_shell *shell);
char							**ft_copy_token_segment(t_shell *shell,
									int start, int end);
int								ft_validate_pipe_syntax(t_shell *shell);
char							*ft_remove_quotes(char *token);
char							*ft_get_env(t_env *env, const char *key);
void							ft_update_env(t_env *env);
void							ft_cd(t_shell *shell, char **args);
int								ft_handle_cd(t_shell *shell);
void							tests(t_shell shell, t_command cmd);
int								ft_is_flag(char *token);
void							ft_args_with_flags(t_command *cmd);
/* parse_args utils */
char							*ft_join_flags(t_command *cmd, int *i);
char							**ft_realloc_flagged(char **flagged, int count);
void							ft_push_flagged(char ***flagged, int *count,
									char *entry);
void							ft_finalize_flagged(char ***flagged, int count);

int								ft_is_builtin(char *cmd);
void							ft_builtin_pwd(void);
void							ft_builtin_echo(char **args);
void							ft_builtin_env(t_shell *shell);
void							ft_builtin_export(t_shell *shell, char **args);
void							ft_builtin_unset(t_shell *shell, char **args);
void							ft_builtin_exit(t_shell *shell, char **args);
void							ft_execute_builtin(t_shell *shell,
									t_command *cmd);

/* export utils */
int								ft_is_valid_identifier(char *str);
void							ft_add_env_var(t_env *env, char *var);
char							*ft_expand_export_value(t_shell *shell,
									char *arg);
void							print_export_sorted(t_env *env);
void							ft_print_export_line(char *var);
void							ft_update_env_var(t_env *env, int index,
									char *var);

void							ft_execute_simple_command(t_shell *shell);
void							ft_execute_pipeline_execve(t_shell *shell);
pid_t							*init_pipeline_resources(t_shell *shell);
void							wait_all_children(t_shell *shell,
									pid_t *child_pids);
int								fork_and_handle(t_shell *shell, int i,
									pid_t **child_pids, int *prev_fd);
pid_t							create_child_process(void);
void							handle_child_in_pipeline(t_shell *shell,
									int i, int prev_fd, int pipe_fd[2]);
void							executor_run(t_shell *shell);
char							*ft_resolve_command_path(t_shell *shell,
									char *cmd);
void							handle_redirections(t_command *cmd);
void							handle_single_redirection(t_redirection *redir);
void							process_heredoc_content(t_redirection *redir);
/* pipeline utils */
int								open_pipe_if_needed(t_command *cmd, int i,
									int total, int pipe_fd[2]);
void							close_if_valid(int fd);
void							setup_child_pipes(int prev_fd, t_command *cmd,
									int pipe_fd[2]);
void							parent_after_fork(pid_t pid, int *prev_fd,
									int pipe_fd[2], t_command *cmd);
void							finalize_status(t_shell *shell,
									int last_status);
void							execve_or_die(char *path, char **argv,
									char **envp);
void							pipe_exec_child(t_shell *shell, t_command *cmd,
									int prev_fd, int pipe_fd[2]);

char							*ft_expand_variable(t_shell *shell,
									char *var_name);
char							*ft_expand_double_quotes(t_shell *shell,
									char *str);
char							*ft_expand_unquoted_token(t_shell *shell,
									char *token);
char							*ft_process_token_quotes(t_shell *shell,
									char *token);
char							*process_token_by_type(t_shell *shell,
									char *token);
char							*build_unquoted_result(t_shell *shell,
									char *token);
char							*ft_extract_var_name(char *str, int *i);
char							*ft_expand_double_quotes(t_shell *shell,
									char *str);
int								ft_is_redirection_token(char *token);
void							handle_redirections_with_heredoc(
									t_command *cmd);
void							handle_heredoc(const char *delimiter);
char							*append_line_to_content(char *content,
									char *line, size_t *len);
char							*read_heredoc_content(const char *delimiter);
int								has_heredoc(t_command *cmd);
void							setup_shell_signals(void);
void							sigint_handler(int signo);
void							heredoc_sigint_handler(int signo);
void							setup_heredoc_signals(void);
void							setup_child_signals(void);
void							apply_redirs(t_command *cmd);
void							update_status_from_wait(t_shell *shell,
									int status);

/* main utils */
int								ft_copy_env_vars(t_shell *shell, char **env);
int								ft_init_env(t_shell *shell, char **env);
int								ft_process_line(t_shell *shell, char *line,
									t_command ***cmds);
void							ft_main_loop(t_shell *shell, char **line,
									t_command ***cmds);

/* executor utils */
int								ft_check_simple_command(t_shell *shell);
void							ft_handle_command_not_found(t_shell *shell,
									char *cmd_name);
void							handle_external_command(t_shell *shell,
									t_command *cmd);
void							execute_external_command(t_shell *shell,
									t_command *cmd);
void							exec_external_in_child(t_shell *shell,
									t_command *cmd);
char							*get_command_path(t_shell *shell,
									t_command *cmd);
pid_t							*ft_init_pipeline(t_shell *shell);
void							ft_handle_pipeline_child(t_shell *shell,
									t_command *cmd, int prev_fd, int *pipe_fd);
void							ft_wait_pipeline(t_shell *shell,
									pid_t *child_pids);

#endif