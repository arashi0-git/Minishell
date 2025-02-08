/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/08 15:11:08 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/*---shell struct---*/
typedef struct s_shell
{
	t_env			*env;
	char			*pwd;
	int				exit_status;
	int				interactive;
}					t_shell;

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
	t_env	*env_list;
	t_env	*new_env;
	int		i;
	char	*equal_sign;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		equal_sign = strchr(env[i], '=');
		if (equal_sign)
		{
			new_env = malloc(sizeof(t_env));
			if (!new_env)
				return (NULL);
			new_env->key = strndup(env[i], equal_sign - env[i]);
			new_env->value = strdup(equal_sign + 1);
			new_env->next = env_list;
			env_list = new_env;
		}
		i++;
	}
	return (env_list);
}

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	if (!shell->env)
	{
		write(STDIN_FILENO, "Failed to initialize environment\n", 34);
		exit(1);
	}
	shell->pwd = getcwd(NULL, 0);
	if (!shell->pwd)
	{
		void set_signal_handlers(void) perror("getcwd");
		free_env(shell->env);
		exit(1);
	}
	shell->exit_status = 0;
	shell->interactive = isatty(STDIN_FILENO);
}

void

	int
	main(int argc, char **argv, char **envp)
{
	char	*input;
	t_shell	shell;

	(void)argv;
	if (argc > 1)
	{
		printf("invalid argument ./minishell\n");
		return (1);
	}
	init_shell(&shell, envp);
	set_signal_handlers();
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break ;
		}
		if (*input != '\0')
		{
			add_history(input);
			printf("%s\n", input);
			process_input(&shell, input);
		}
		free(input);
	}
	return (0);
}
