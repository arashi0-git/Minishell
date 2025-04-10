/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:23:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/22 16:32:17 by marvin           ###   ########.fr       */
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
	int				pipeline_pipe[2];

	pipeline_pipe[0] = -1;
	pipeline_pipe[1] = -1;
	*last_pid = -1;
	cmd = cmd_list;
	while (cmd != NULL)
	{
		init_pipe_state(&state, cmd);
		execute_command(shell, cmd, state, pipeline_pipe);
		if (state == PIPE_READ_ONLY || state == NO_PIPE)
			*last_pid = cmd->pid;
		cmd = cmd->next;
	}
}

void	wait_last_command(t_shell *shell, t_cmd *cmd_list, pid_t last_pid)
{
	t_cmd	*current;
	int		status;

	current = cmd_list;
	while (current)
	{
		if (current->pid > 0)
		{
			status = wait_for_command(current->pid);
			if (current->pid == last_pid)
				shell->exit_status = status;
		}
		current = current->next;
	}
}

void	free_cmd(t_cmd *cmd)
{
	int	i;

	if (cmd->args)
	{
		i = 0;
		while (i < cmd->argc)
			free(cmd->args[i++]);
		free(cmd->args);
	}
	if (cmd->command)
		free(cmd->command);
	if (cmd->infile)
		free(cmd->infile);
	if (cmd->outfile)
		free(cmd->outfile);
	if (cmd->heredoc_delims)
		ft_lstclear(&cmd->heredoc_delims, free);
	if (cmd->redirects)
		free_redirect(cmd->redirects);
	free(cmd);
}
