#include "../../include/minishell.h"
#include "../../include/execution.h"

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
	if (!cmd->next)
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
		close(old_pipe[PIPE_IN]);
		close(old_pipe[PIPE_OUT]);
	}
	if (state == PIPE_WRITE_ONLY || state == PIPE_READ_WRITE)
	{
		close(new_pipe[PIPE_OUT]);
		old_pipe[PIPE_IN] = new_pipe[PIPE_IN];
		old_pipe[PIPE_OUT] = new_pipe[PIPE_OUT];
	}
}
