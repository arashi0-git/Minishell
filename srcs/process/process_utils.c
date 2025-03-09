/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:23:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/10 01:43:22 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/parse.h"
#include <sys/wait.h>

void	execute_commands(t_shell *shell, t_cmd *cmd_list, pid_t *last_pid)
{
	t_cmd			*cmd;
	t_pipe_state	state;
	int				status;
	int				pipeline_pipe[2];

	pipeline_pipe[0] = -1;
	pipeline_pipe[1] = -1;
	*last_pid = -1;
	cmd = cmd_list;
	while (cmd != NULL)
	{
		init_pipe_state(&state, cmd);
		status = execute_command(shell, cmd, state, pipeline_pipe);
		if (state == PIPE_READ_ONLY)
			*last_pid = cmd->pid;
		cmd = cmd->next;
	}
}

void	wait_last_command(t_shell *shell, t_cmd *cmd_list, pid_t last_pid)
{
	int	status;

	if (cmd_list && cmd_list->next && last_pid > 0)
	{
		if (waitpid(last_pid, NULL, WNOHANG) == 0)
		{
			status = wait_for_command(last_pid);
			shell->exit_status = status;
		}
	}
}
