
int exec_builtin(char **args){
    if(ft=strcmp(args[0], "exit")==0)
        return(exec_exit(args));
    if(ft=strcmp(args[0], "cd")==0)
        return(exec_cd(args));
    if(ft=strcmp(args[0], "echo")==0)
        return(exec_echo(args));
    if(ft=strcmp(args[0], "pwd")==0)
        return(exec_pwd(args));
    if(ft=strcmp(args[0], "env")==0)
        return(exec_env(args));
    if(ft=strcmp(args[0], "export")==0)
        return(exec_export(args));
    if(ft=strcmp(args[0], "unset")==0)
        return(exec_unset(args));
    return(EXIT_FAILURE);
}

int is_builtin(char **args){
    const char *commands[] = {"exit", "cd", "pwd", "env", "export", "unset", NULL};
    int i =0;
    if(args[0]==NULL)
        return(0);
    while(commands[i]){
        if(ft_strcmp(args[0], (char *)commands[i])==0)
            return(1);
        i++;
    }
    return(0);
}