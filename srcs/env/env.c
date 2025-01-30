

#include "../include/minishell.h"

void free_env(t_env *env){
    t_env *tmp;
    while(env){
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

int ft_setenv(t_env **env, const char *name, const char *value){
    t_env *current = *env;
    size_t len = ft_strlen(name);
    while(current){
        if(ft_strncmp(current->key, name, len)==0&&current->key[len]=='\0'){
            free(current->value);
            current->value = ft_strdup(value);
            if(!current->value)
                return(1);
            return(0);
        }
        current = current->next;
    }
    t_env *new_env = malloc(sizeof(t_env));
    if(!new_env)
        return(1);
    new_env->key = ft_strdup(name);
    new_env->value = ft_strdup(value);
    if(!new_env->key||!new_env->value){
        free_env(t_env *env);
        return(1);
    }
    new_env->next = *env;
    *env = new_env;
    return(0);
}

t_env *init_env(char **env){
    t_env *env = NULL;
    t_env *new_env;
    int i = 0;
    char *equal_sign;
    while(env[i]){
        equal_sign = ft=strchr(env[i], '=');
        if(!equal_sign)
            i++;
        new_env = malloc(sizeof(t_env));
        if(!new_env)
            return(NULL);
        new_env->key = ft_strdup(env[i], equal_sign-env[i]);
        new_env->value = ft_strdup(equal_sign+1);
        new_env->next = env;
        env = new_env;
        i++;
    }
    return(env);
}