/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:23:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/28 16:40:15 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/*---module---*/
typedef struct s_cmd	t_cmd;
typedef struct s_token	t_token;
# include "builtin.h"
# include "expand.h"
# include "parse.h"
# include "tokenize.h"

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
# include <unistd.h>

# define PATH_MAX 4096




/*---env struct---*/
typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;



typedef struct s_builtin
{
	char				*name;
	int					(*func)(char **args, t_shell *shell);
}						t_builtin;

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

	t_env				*env;
	char				*pwd;
	int					exit_status;
	int					interactive;
}						t_shell;



/*---env func---*/
t_env					*init_env(char **env);
void					free_env(t_env *env);

/*---signal func---*/
void					set_signal_handlers(void);

/*---process func---*/
void					process_input(t_shell *shell, char *input);
void					free_cmd_list(t_cmd *cmd_list);

/*---builtin---*/
void					exec_echo(char **args);
int						exec_cd(char **args, t_shell *shell);
int						process_pwd(void);

#endif
