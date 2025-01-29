#include "../include/minishell.h"

init_shell(t_shell shell, char **env){
    
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