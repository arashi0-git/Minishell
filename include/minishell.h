/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/28 15:56:14 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

# define REDIR_IN 0
# define REDIR_OUT 1
# define REDIR_APPEND 2
# define REDIR_HEREDOC 3
# define PATH_MAX 4096

typedef struct stat	t_stat;

typedef enum e_bool {
    FALSE = 0,
    TRUE = 1
} t_bool;

/*---tokenize---*/
typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_PIPE,
	TOKEN_REDIR
}					t_tokentype;

typedef enum redirect_type
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}					t_redirecttype;

typedef struct s_token
{
	char			*value;
	t_tokentype		type;
	t_redirecttype	redirtype;
	struct s_token	*next;
}					t_token;


typedef struct s_cmd
{
	char			*command;
	char			**args;
	int				argc;
	int				max_args;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
    struct s_cmd    *prev;          // パイプライン用
    pid_t           pid;            // プロセス管理用
    struct s_redirect *redirects;      // リダイレクト管理用
}					t_cmd;

/*---expand struct---*/
typedef struct s_expand
{
	char			*out;
	size_t			i;
	size_t			out_index;
}					t_expand;

/*---env struct---*/
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;



typedef struct s_pipe
{
	int					fd[2];
	struct s_pipe		*next;
}						t_pipe;


int	set_env(t_env **env, const char *key, const char *value);
char **sort_env(t_env *env);
int     print_sorted_env(t_env *env);
int	set_env(t_env **env, const char *key, const char *value);
void free_array(char **array);
/*---shell struct---*/
typedef struct s_shell
{
	t_env			*env;
	char			*pwd;
	int				exit_status;
	int				interactive;
}					t_shell;


typedef struct s_builtin
{
	char				*name;
	int					(*func)(char **args, t_shell *shell);
}						t_builtin;

/*---tokenize func---*/
t_token				*tokenize_list(char *line);
//char		(char **p, t_tokentype *token_type);

/*---parse func---*/
t_cmd				*parse_tokens(t_token *tokens);
int					handle_redirection(t_cmd *cmd, t_token **curr_ptr);
int					add_arg(t_cmd *cmd, const char *arg);
t_cmd				*new_cmd(void);

/*---env func---*/
t_env				*init_env(char **env);
void				free_env(t_env *env);
char	*get_env(t_env *env, const char *name);
int	set_env(t_env **env, const char *key, const char *value);

/*---tokenize func---*/
t_token				*tokenize_list(char *line);
char				*get_token(char **p, t_tokentype *token_type);
/*---signal func---*/
void				set_signal_handlers(void);

/*---builtin func---*/
int					is_builtin(char **args);
int					exec_builtin(char **args, t_shell *shell);

/*---process func---*/
void				process_input(t_shell *shell, char *input);
void				free_cmd_list(t_cmd *cmd_list);
void       free_token_list(t_token *list);

/*---expand func---*/
void				expand_cmd(t_cmd *cmd, t_shell *shell);
int					process_dollar_others(const char *str, size_t *i,
						size_t *len);
int					process_dollar_question(t_shell *shell, size_t *i,
						size_t *len);
int					process_quote_expand(const char *str, t_shell *shell,
						size_t *i, size_t *len);
int					process_dollar_length(const char *str, t_shell *shell,
						size_t *i, size_t *len);
int					process_expansion_char(const char *str, t_shell *shell,
						t_expand *exp);
t_expand			*init_expand(size_t total_len);
int					expand_dollar_question(t_shell *shell, t_expand *exp);
int					expand_dollar_variable(const char *str, t_expand *exp);

/*---error func---*/
void	print_error(char *message, char *command);

/***exit func***/
void	error_exit(char *command);


/*---execution func---*/
int	execute_command(t_shell *shell, t_cmd *cmd);
#endif
