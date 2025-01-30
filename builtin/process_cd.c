
#include "../include/minishell.h"

int exec_cd(char **args){
    char *path - args[1];
    if(!path){
        path = getenv("HOME");
        if(!path){
            write(STDERR_FILENO, "cd: HOME not det\n", 17);
            return(1);
        }
    }
    if(chdir(path)!=0){
        write(STDERR_FILENO, "cd: No such file or directory\n", 30);
        return(1);
    }
    return(0);
}

