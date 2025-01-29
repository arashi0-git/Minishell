#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>

#define REDIR_IN 0;
#define REDIR_OUT 1;
#define REDIR_APPEND 2;
#define REDIR_HEREDOC 3;

typedef struct s_shell{
    char **env;
    int exit_status;
    int interactive;
} t_shell;

typedef struct t_token{
    char *value;
    int type;
    struct s_token *next;
} t_token;

typedef struct s_command{
    char **args;
    t=redir *redirect;
    struct s_command *next;
} t_command;

typedef struct s_redir{
    int type;
    char *file;
    struct s_redir *next;
} t_redir;

typedef struct s_env{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_builtin{
    char *name;
    int (*func)(char **args, t_shell *shell);
} t_builtin;

typedef struct s_pipe{
    int fd[2];
    struct s_pipe *next;
} t_pipe;

#endif