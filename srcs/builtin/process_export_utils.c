/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:23:55 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/11 14:33:40 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_export_format(const char *env_str)
{
	char	*equal_pos;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	equal_pos = ft_strchr(env_str, '=');
	if (equal_pos)
	{
		write(STDOUT_FILENO, env_str, equal_pos - env_str);
		ft_putstr_fd("=\"", STDOUT_FILENO);
		ft_putstr_fd(equal_pos + 1, STDOUT_FILENO);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	else
		ft_putstr_fd(env_str, STDOUT_FILENO);
	ft_putstr_fd("\n", STDOUT_FILENO);
}

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

int	print_sorted_env(t_env *env)
{
	char	**sorted_env;
	int		i;

	sorted_env = sort_env(env);
	if (!sorted_env)
		return (1);
	i = 0;
	while (sorted_env[i])
	{
		print_export_format(sorted_env[i]);
		i++;
	}
	free_array(sorted_env);
	return (0);
}
