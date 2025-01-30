#include "../include/minishell.h"

void handle_sigint(int sig){
    (void)sig;
    printf('\n');
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigout(int sig){
    (void)sig;
}

void set_siganl_handlers(void){
    signal(SIGINT, handle_sigint);
    signal(SIGOUT, handle_sigquit);
}

char **copy_env(char **env){
    int i = 0;
    while(env[i])
        i++;
    char **new_env = malloc((i+1)*sizeof(char *));
    if(!new_env)
        return(NULL);
    int j=0;
    while(j<i){
        new_env[j] = ft_strdup(env[j]);
        if(!new_env){
            while(j<0){
                free(new_env[j]);
                j--;
            }
            free(new_env);
            return(NULL);
        }
        j++;
    }
    new_env[i]= NULL;
    return(new_env);

}

init_shell(t_shell *shell, char **env){
    shell->env = copy_env(env);
    if(!shell->env){
        perror("failed to initialize environment");
        exit(1);
    }
    shell->pwd = getcwd(NULL, 0);
    shell->exit_status = 0;
    shell->interactive = isatty(STDIN_FILENO);
    if(shell->interactive)
    printf("Minishell started in interactive mode\n");
}

int main(int argc, char **argv, char **env){
    t_shell shell;
    init_shell(&shell, env);
    set_siganl_handlers();
    while(1){
        char *input = readline("minishell> ");
        if(!input){
            printf("exit\n");
            break;
        }
        if(*input != '\0'){
            add_history(input);
            process_input(&shell, input);
        }
        free(input);
    }
    free_shell(&shell);
    return(0);
}