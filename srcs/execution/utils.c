/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 16:38:06 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/06 20:05:00 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

t_bool	can_generate_environ(t_env *env)
{
	if (env == NULL)
		return (false);
	if (env->key == NULL)
		return (false);
	return (true);
}

size_t	get_environ_size(t_env *envs)
{
	size_t	size;

	size = 0;
	while (envs)
	{
		if (can_generate_environ(envs))
			size++;
		envs = envs->next;
	}
	return (size);
}
