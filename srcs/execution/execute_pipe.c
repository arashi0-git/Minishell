/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/09 22:16:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/minishell.h"
#include "../../include/parse.h"

void	create_pipe(t_pipe_state state, int new_pipe[])
{
	if (state == PIPE_WRITE_ONLY || state == PIPE_READ_WRITE)
	{
		if (pipe(new_pipe) < 0)
			error_exit(NULL);
	}
}

void	setup_pipes(t_pipe_state state, int old_pipe[2], int new_pipe[2])
{
	if (state == PIPE_READ_ONLY || state == PIPE_READ_WRITE)
	{
		if (dup2(old_pipe[PIPE_IN], STDIN_FILENO) < 0)
			error_exit(NULL);
	}
	if (state == PIPE_WRITE_ONLY || state == PIPE_READ_WRITE)
	{
		if (dup2(new_pipe[PIPE_OUT], STDOUT_FILENO) < 0)
			error_exit(NULL);
	}
}

void	init_pipe_state(t_pipe_state *state, t_cmd *cmd)
{
	if (!cmd->next && !cmd->prev)
		*state = NO_PIPE;
	else if (!cmd->prev)
		*state = PIPE_WRITE_ONLY;
	else if (!cmd->next)
		*state = PIPE_READ_ONLY;
	else
		*state = PIPE_READ_WRITE;
}

void	cleanup_pipe(t_pipe_state state, int old_pipe[2], int new_pipe[2])
{
	if (state == PIPE_READ_ONLY || state == PIPE_READ_WRITE)
	{
		if (old_pipe[PIPE_IN] != -1)
			close(old_pipe[PIPE_IN]);
		if (old_pipe[PIPE_OUT] != -1)
			close(old_pipe[PIPE_OUT]);
	}
	if (state == PIPE_WRITE_ONLY || state == PIPE_READ_WRITE)
	{
		if (new_pipe[PIPE_OUT] != -1)
			close(new_pipe[PIPE_OUT]);
		old_pipe[PIPE_IN] = new_pipe[PIPE_IN];
		old_pipe[PIPE_OUT] = -1;
	}
}
