/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:03:31 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/15 11:25:24 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	remove_env_unset(t_shell *shell, char *arg)
{
	t_env	*current;
	t_env	*prev;

	if (ft_strcmp(shell->env->key, arg) == 0)
	{
		current = shell->env;
		shell->env = shell->env->next;
		free_env_node(current);
		return ;
	}
	prev = shell->env;
	current = shell->env->next;
	while (current && ft_strcmp(current->key, arg) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (current)
	{
		prev->next = current->next;
		free_env_node(current);
		return ;
	}
}

int	exec_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
		return (0);
	i = 1;
	status = 0;
	if (!shell->env)
		return (1);
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			status = 0;
		}
		else
			remove_env_unset(shell, args[i]);
		i++;
	}
	shell->exit_status = 0;
	return (status);
}
