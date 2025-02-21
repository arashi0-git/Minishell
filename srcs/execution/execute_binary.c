#include "../../include/minishell.h"

// executor_binary.c
void exec_binary(t_shell *shell, char **args)
{
    char    *path;
    char    **envp;

    envp = create_environ(shell->env);
    path = find_commond_path(args[0], envp);
    
    if (execve(path, args, envp) < 0)
        handle_execve_error(path);
        
    free(path);
    ft_free_array(envp);
}

