
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
        target = get_env(*env, "HOME");
        if(!target){
            print_error("cd: HOME not set", NULL);
            return(1);
        }
    }
    else if(ft_strcmp(path, "-")==0){
        target = get_env(shell->env, "OLDPWD");
        if(!target){
            print_error("cd: OLDPWD not set", NULL);
            return(1);
        }
        write(STDOUT_FILENO, target, ft_strlen(target));
        write(STDOUT_FILENO, "\n", 1);
    }
    else
        target = path;
    if(chdir(target)!=0){
        perror("cd");
        return(1);
    }
    if(getcwd(newpwd, sizeof(newpwd))== NULL){
        perror("cd: getcwd");
        return(1);
    }
    if(set_env(&(shell->env), "OLDPWD", pldpwd)!=0){
        print_error("cd: Failed to set OLDPWD", NULL);
        return(1);
    }
    if(set_env(&(shell->env), "PWD", pldpwd)!=0){
        print_error("cd: Failed to set PWD", NULL);
        return(1);
    }
    return(0);
}

