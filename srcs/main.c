/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/08 17:02:31 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	init_shell(t_shell *shell, char **envp)
{
	shell->env = init_env(envp);
	if (!shell->env)
	{
		printf("initialize shell failed\n");
		exit(EXIT_FAILURE);
	}
	shell->pwd = getcwd(NULL, 0);
	if (!shell->pwd)
	{
		void set_signal_handlers(void) perror("getcwd");
		free_env(shell->env);
		exit(EXIT_FAILURE);
	}
	shell->exit_status = 0;
	shell->interactive = isatty(STDIN_FILENO);
}

void	minishell(t_shell *shell)
{
	char	*input;

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
			process_input(shell, input);
		}
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argv;
	if (argc > 1)
	{
		printf("invalid argument ./minishell\n");
		return (1);
	}
	init_shell(&shell, envp);
	set_signal_handlers();
	minishell(&shell);
	free_shell(&shell);
	return (0);
}
