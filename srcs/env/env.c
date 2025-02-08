/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 14:56:27 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/08 14:56:46 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
