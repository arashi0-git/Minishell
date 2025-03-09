/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:18:21 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/09 21:43:57 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static char	*parse_env_argument(char *args, char **value, int *append_mode)
{
	char	*args_copy;
	char	*equal_pos;

	*append_mode = 0;
	args_copy = ft_strdup(args);
	equal_pos = ft_strchr(args_copy, '=');
	if (equal_pos)
	{
		if (equal_pos != args_copy && *(equal_pos - 1) == '+')
		{
			*append_mode = 1;
			*(equal_pos - 1) = '\0';
		}
		else
			*equal_pos = '\0';
		*value = equal_pos + 1;
	}
	else
	{
		*value = NULL;
	}
	return (args_copy);
}

static void	update_env_variable_append(t_env **env, char *key, char *value)
{
	char	*old_value;
	char	*new_value;

	old_value = get_env_value(*env, key);
	if (old_value)
	{
		new_value = ft_strjoin(old_value, value);
		set_env(env, key, new_value);
		free(new_value);
	}
	else
	{
		set_env(env, key, value);
	}
}

void	update_env_variable(char *args, t_env **env)
{
	char	*key;
	char	*value;
	char	*key_alloc;
	int		append_mode;

	if (!args || !env)
		return ;
	key_alloc = parse_env_argument(args, &value, &append_mode);
	key = key_alloc;
	if (append_mode)
		update_env_variable_append(env, key, value);
	else
		set_env(env, key, value);
	free(key_alloc);
}

int	exec_export(char **cmd_and_args, t_shell *shell)
{
	int	i;
	int	ret;
	int	res;

	ret = 0;
	if (!cmd_and_args[1])
		return (print_sorted_env(shell->env));
	i = 1;
	while (cmd_and_args[i])
	{
		res = process_export_argument(cmd_and_args[i], shell);
		if (res != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
