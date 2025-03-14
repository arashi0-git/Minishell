/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:23:55 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/09 21:44:33 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
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

int	get_env_size(t_env *env)
{
	int		size;
	t_env	*current;

	size = 0;
	current = env;
	while (current)
	{
		size++;
		current = current->next;
	}
	return (size);
}

int	process_export_argument(char *arg, t_shell *shell)
{
	if (arg[0] == '\0')
		return (0);
	if (!is_valid_identifier(arg))
	{
		printf("minishell: export: `%s': not a valid identifier\n", arg);
		shell->exit_status = 1;
		return (1);
	}
	update_env_variable(arg, &shell->env);
	return (0);
}
