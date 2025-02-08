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

# define REDIR_IN 0
# define REDIR_OUT 1
# define REDIR_APPEND 2
# define REDIR_HEREDOC 3
# define PATH_MAX 4096

/*---AST---*/
typedef enum
{
	NODE_COMMAND,
	NODE_PIPE,
	NODE_SEQUENCE
}						nodetype;

typedef enum
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}						RedirectType;

typedef struct s_redirect
{
	RedirectType		type;
	char				*filename;
	struct t_redirect	*next;
}						t_redirect;

typedef struct s_command
{
	char				**args;
	int					argcount;
	t_redirect			*redirects;
	bool				is_builtin;
}						t_command;

typedef struct ASTnode
{
	nodetype			type;
	t_command			command;
	struct ASTnode		*left;
	struct ASTnode		*right;
}						ASTnode;

/*---parser struct---*/
typedef struct s_parser
{
	char				**tokens;
	int					token_count;
	int					pos;
}						t_parser;

/*---SHELL---*/
typedef struct s_shell
{
	char				**env;
	char				*pwd;
	int					exit_status;
	int					interactive;
}						t_shell;

typedef struct s_token
{
	char				*value;
	int					type;
	struct s_token		*next;
}						t_token;

typedef struct s_redir
{
	int					type;
	char				*file;
	struct t_redir		*next;
}						t_redir;

/*二重定義のため、一旦コメントアウトしました
typedef struct s_command
{
	char				**args;
	t_redir				*redirect;
	struct s_command	*next;
}						t_command;
*/

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

/*---tokenize---*/
char					**token_split(char *str);
int						count_tokens(char *str);
char					*get_token(char **p);
#endif
