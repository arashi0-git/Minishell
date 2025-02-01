/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:17 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/01 11:05:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_input(t_shell *shell, char *input)
{
	char **args;
	int i = 0;

	args = ft_split(input, ' ');
	if (!args)
	{
		write(STDERR_FILENO, "Memory allocation error\n", 24);
		return ;
	}
	if (is_builtin(args))
		shell->exit_status = exec_builtin(args, shell);
	else
	{
		pid_t pid = fork();
		if (pid < 0)
			write(STDERR_FILENO, "Fork failed\n", 12);
		else if (pid == 0)
		{
			int env_count = 0;
			t_env *current = shell->env;
			while (current)
			{
				env_count++;
				current = current->next;
			}
			char **env = malloc((env_count + 1) * sizeof(char *));
			if (!env)
			{
				write(STDERR_FILENO, "Memory allocaton error\n", 24);
				exit(EXIT_FAILURE);
			}
			current = shell->env;
			i = 0;
			while (i < env_count)
			{
				size_t key_len = ft_strlen(cuurent->key);
				size_t value_len = ft_strlen(cuurent->value);
				env[i] = malloc(key_len + value_len + 2);
				if (env[i])
				{
					write(STDERR_FILENO, "Memory allocation error\n", 24);
					while (i > 0)
						free(env[--i]);
					free(env);
					exit(EXIT_FAILURE);
				}
			}
		}
	}
}