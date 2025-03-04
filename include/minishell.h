/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/04 20:09:47 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*---module---*/
typedef struct s_cmd	t_cmd;
typedef struct s_token	t_token;

/*
# include "builtin.h"
# include "expand.h"
# include "parse.h"
# include "tokenize.h"
*/
/*---library---*/
# include <errno.h>
# include <libft.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

# define PATH_MAX 4096

typedef struct stat		t_stat;

typedef enum e_bool
{
	FALSE = 0,
	TRUE = 1
}						t_bool;

/*---env struct---*/
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_pipe
{
	int					fd[2];
	struct s_pipe		*next;
}						t_pipe;

int						set_env(t_env **env, const char *key,
							const char *value);
char					**sort_env(t_env *env);
int						print_sorted_env(t_env *env);
int						set_env(t_env **env, const char *key,
							const char *value);
void					free_array(char **array);
/*---shell struct---*/
typedef struct s_shell
{
	t_env				*env;
	char				*pwd;
	int					exit_status;
	int					interactive;
}						t_shell;

typedef struct s_builtin
{
	char				*name;
	int					(*func)(char **args, t_shell *shell);
}						t_builtin;

/*---env func---*/
t_env					*init_env(char **env);
void					free_env(t_env *env);
char					*get_env(t_env *env, const char *name);
int						set_env(t_env **env, const char *key,
							const char *value);

/*---signal func---*/
void					set_signal_handlers(void);

/*---process func---*/
void					process_input(t_shell *shell, char *input);
void					free_cmd_list(t_cmd *cmd_list);
void					free_token_list(t_token *list);
int						is_valid_identifier(const char *str);

/*---error func---*/
void					print_error(char *message, char *command,
							char *command2);

/***exit func***/
void					error_exit(char *command);

/*---execution func---*/
void					process_input(t_shell *shell, char *input);
void					free_cmd_list(t_cmd *cmd_list);

#endif
