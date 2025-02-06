/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/06 22:25:31 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

char	*get_env(t_env *env, const char *name)
{
	size_t	len;

	len = ft_strlen(name);
	while (env)
	{
		if (ft_strncmp(env->key, name, len) == 0 && env->key[len] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	set_env(t_env **env, const char *name, const char *value)
{
	t_env	*current;
	size_t	len;
	t_env	*new_env;

	current = *env;
	len = ft_strlen(name);
	while (current)
	{
		if (ft_strncmp(current->key, name, len) == 0
			&& current->key[len] == '\0')
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (1);
			return (0);
		}
		current = current->next;
	}
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return (1);
	new_env->key = ft_strdup(name);
	new_env->value = ft_strdup(value);
	if (!new_env->key || !new_env->value)
	{
		free_env(*env);
		return (1);
	}
	new_env->next = *env;
	*env = new_env;
	return (0);
}

t_env	*init_env(char **env)
{
	t_env	*env_list;
	t_env	*new_env;
	int		i;
	char	*equal_sign;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		equal_sign = ft_strchr(env[i], '=');
		if (equal_sign)
		{
			new_env = malloc(sizeof(t_env));
			if (!new_env)
				return (NULL);
			new_env->key = ft_strndup(env[i], equal_sign - env[i]);
			new_env->value = ft_strdup(equal_sign + 1);
			new_env->next = env_list;
			env_list = new_env;
		}
		i++;
	}
	return (env_list);
}

/*
 //libft をmake & env/でcc -Wall -Wextra -Werror env.c -I../../include -L../../libft -lft でテスト可能
int	main(int argc, char *argv[], char **envp)
{
	t_env	*env_list;
	t_env	*current;

	(void)argc;
	(void)argv;
	env_list = init_env(envp);
	if (!envp)
	{
		printf("初期化失敗\n");
		return (1);
	}
	current = env_list;
	while (current)
	{
		printf("key: %s, value: %s\n", current->key, current->value);
		current = current->next;
	}
	free_env(env_list);
	return (0);
}

 init_envがすべきこと：
プログラム起動時に、親プロセス（通常のシェル）から渡される環境変数の配列（char **envp）を受け取る
その環境変数を、minishellで使いやすい形（連結リスト）に変換して保存する
*/
