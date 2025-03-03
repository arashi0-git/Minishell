/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/03 16:23:06 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_shell	*g_shell = NULL;

void	free_shell(t_shell *shell)
{
	if (!shell)
		return ;
	free_env(shell->env);
	free(shell->pwd);
}

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
		perror("getcwd");
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
			write(STDERR_FILENO, "exit\n", 5);
			shell->exit_status = 130;
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
	g_shell = &shell;
	set_signal_handlers();
	minishell(&shell);
	free_shell(&shell);
	return (0);
}
