
#include "../../include/minishell.h"

int exec_echo(char **args){
    int i =1;
    int newline = 1;
    if(args[i]&&ft_strcmp(args[i], "-n")==0){
        newline = 0;
        i++;
    }
    while(args[i]){
        write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
        if(args[i+1])
            write(STDOUT_FILENO, " ", 1);
        i++;
    }
    if(newline)
        write(STDOUT_FILENO, "\n", 1);
    return(0);
}