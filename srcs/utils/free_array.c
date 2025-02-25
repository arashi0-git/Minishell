/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:45:38 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/23 19:46:10 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_array(char **sorted_env)
{
	int	i;

	i = 0;
	if (!sorted_env)
		return ;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

