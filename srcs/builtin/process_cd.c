
#include "../include/minishell.h"

int exec_cd(char **args){
    char *path = args[1];
    char oldpwd[PATH_MAX];
    char newpwd[PATH_MAX];
    char *target;

    if(getcwd(oldpwd, sizeof(oldpwd))==NULL){
        perror("cd: getcwd");
        return(1);
    }
    if(!path){
        target = getenv(*env, "HOME");
    }
}

