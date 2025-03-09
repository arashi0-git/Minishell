/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 16:23:30 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/09 19:47:54 by aryamamo         ###   ########.fr       */
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
