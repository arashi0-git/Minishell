/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 12:43:51 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/11 12:05:15 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

extern t_shell	*g_shell;

void	handle_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	if (g_shell != NULL)
		g_shell->exit_status = 130;
}

void	set_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &sa_int, NULL) < 0)
	{
		perror("sigaction SIGINT");
		exit(1);
	}
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	if (sigaction(SIGQUIT, &sa_quit, NULL) < 0)
	{
		perror("sigaction SIGQUIT");
		exit(1);
	}
}

// int	main(int argc, char **argv)
// {
// 	char	*input;

// 	(void)argc;
// 	(void)argv;
// 	set_signal_handlers();
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			write(STDOUT_FILENO, "exit\n", 5);
// 			break ;
// 		}
// 		if (*input != '\0')
// 		{
// 			add_history(input);
// 			printf("%s\n", input);
// 		}
// 		free(input);
// 	}
// 	return (0);
// }
