#include "../include/minishell.h"

void handle_sigint(int sig){
    (void)sig;
    printf('\n');
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int sig){
    (void)sig;
}

void set_siganl_handlers(void){
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}

init_shell(t_shell *shell, char **env){
    shell->env = init_env(env);
    if(!shell->env){
        write(STDERR_FILENO, "failed to initialize environment\n", 32);
        exit(1);
    }
    shell->pwd = getcwd(NULL, 0);
    if(!shell->pwd){
        perror("getcwd");
        free_env(shell->env);
        exit(1);
    }
    shell->exit_status = 0;
    shell->interactive = isatty(STDIN_FILENO);
    if(shell->interactive)
        write(STDOUT_FILENO, "Minishell started in interactive mode\n", 39);
}

void free_shell(t_shell *shell){
    free_env(shell->env);
    free(shell->pwd);
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