/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:03:31 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/11 16:57:07 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static	void	free_env_node(t_env* node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	remove_env_unset(t_shell *shell, char *arg)
{
	t_env *current;
	t_env *prev;

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

int    cmd_unset(char **args, t_shell *shell)
{
    int        i;
    int        status;

    if (!args[1])
        return (0);  // 引数がない場合は成功として扱う

    i = 1;
    status = 0;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            error_invalid_identifier("unset", args[i]);
            status = 1;
        }
        else
            remove_env_var(shell, args[i]);
        i++;
    }
    return (status);
}

