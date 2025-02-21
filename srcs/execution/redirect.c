#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include "exec.h"
#include "expander.h"

#define FILE_MODE 0644


// リダイレクトの種類
typedef enum e_redirect_type {
    REDIR_INPUT,      // 
    REDIR_OUTPUT,     // >
    REDIR_APPEND      // >>
} t_redirect_type;

// トークン構造体
typedef struct s_token {
    char            *data;
    struct s_token  *next;
} t_token;

// リダイレクト構造体
typedef struct s_redirect {
    t_redirect_type    type;
    t_token           *filename;
    int               fd_io;
    int               fd_file;
    int               fd_backup;
    struct s_redirect *next;
    struct s_redirect *prev;
} t_redirect;

// エラー処理用の型
typedef enum e_bool {
    FALSE = 0,
    TRUE = 1
} t_bool;


static int  open_file(t_redirect *redir)
{
    char    *filename;

    filename = redir->filename->data;
    if (redir->type == REDIR_INPUT)
        return (open(filename, O_RDONLY));
    if (redir->type == REDIR_OUTPUT)
        return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));
    return (open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_MODE));
}

static t_bool check_redirect(t_redirect *redir, char *org_filename)
{
    if (redir->filename == NULL || redir->filename->next)
    {
        print_error("ambiguous redirect", org_filename);
        return (FALSE);
    }
    if ((redir->fd_file = open_file(redir)) < 0)
    {
        print_error(strerror(errno), redir->filename->data);
        return (FALSE);
    }
    return (TRUE);
}

t_bool  setup_redirects(t_command *command)
{
    t_redirect   *redir;
    char        *org_filename;

    redir = command->redirects;
    while (redir)
    {
        if ((org_filename = ft_strdup(redir->filename->data)) == NULL)
            error_exit(NULL);
        expand_tokens(&redir->filename);
        if (check_redirect(redir, org_filename) == FALSE)
        {
            free(org_filename);
            cleanup_redirects(command);
            return (FALSE);
        }
        free(org_filename);
        redir = redir->next;
    }
    return (TRUE);
}

void    cleanup_redirects(t_command *command)
{
    t_redirect   *redir;

    redir = command->redirects;
    while (redir && redir->next)
        redir = redir->next;
    while (redir)
    {
        if (redir->fd_file >= 0)
        {
            if (close(redir->fd_file) < 0)
                error_exit(NULL);
        }
        if (redir->fd_backup >= 0)
        {
            if (dup2(redir->fd_backup, redir->fd_io) < 0 ||
                close(redir->fd_backup) < 0)
            {
                error_exit(NULL);
            }
        }
        redir = redir->prev;
    }
}

t_bool  dup_redirects(t_command *command, t_bool is_parent)
{
    t_redirect   *redir;

    redir = command->redirects;
    while (redir)
    {
        if (is_parent)
        {
            if ((redir->fd_backup = dup(redir->fd_io)) < 0)
            {
                print_bad_fd_error(redir->fd_io);
                return (FALSE);
            }
        }
        if (dup2(redir->fd_file, redir->fd_io) < 0)
        {
            print_bad_fd_error(redir->fd_io);
            return (FALSE);
        }
        redir = redir->next;
    }
    return (TRUE);
}

