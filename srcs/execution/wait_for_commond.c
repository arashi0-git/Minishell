/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_for_commond.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/15 12:16:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <sys/wait.h>

int	wait_for_command(pid_t pid)
{
	int	status;
	int	sig;

	if (pid <= 0)
		return (0);
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + sig);
	}
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
