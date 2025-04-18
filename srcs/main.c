/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/22 16:55:08 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int		g_signal = 0;

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
		if (g_signal == 130)
		{
			shell->exit_status = 130;
			g_signal = 0;
		}
		if (!input)
		{
			if (!shell->interactive)
				minishell_pipe(shell);
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
	set_signal_handlers();
	minishell(&shell);
	free_shell(&shell);
	return (0);
}
