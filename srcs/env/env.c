/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/07 16:59:15 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

t_env	*init_env(char **env)
{
	t_env	*env;
	t_env	*new_env;
	int		i;
	char	*equal_sign;

	env = NULL;
	i = 0;
	while (env[i])
	{
		equal_sign = strchr(env[i], '=');
		if (!equal_sign)
		{
			i++;
			continue ;
		}
		new_env = malloc(sizeof(t_env));
		if (!new_env)
			return (NULL);
		new_env->key = strndup(env[i], equal_sign - env[i]);
		new_env->value = strdup(equal_sign + 1);
		new_env->next = env;
		env = new_env;
		i++;
	}
	return (env);
}
