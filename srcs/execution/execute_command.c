/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/15 12:05:18 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <sys/wait.h>

static int	exec_builtin_parent(t_shell *shell, t_cmd *command, char **args)
{
	int	result;

	result = 0;
	if (setup_redirects(command) == FALSE)
		return (EXIT_FAILURE);
	if (dup_redirects(command, TRUE) == FALSE)
		return (EXIT_FAILURE);
	result = exec_builtin(args, shell);
	cleanup_redirects(command);
	return (result);
}

static void	execute_in_child(t_shell *shell, t_cmd *cmd, t_pipe_info *pipe_info)
{
	reset_signal_in_child();
	setup_pipes(pipe_info->state, pipe_info->old_pipe, pipe_info->new_pipe);
	if (!setup_redirects(cmd))
		exit(EXIT_FAILURE);
	if (!dup_redirects(cmd, FALSE))
		exit(EXIT_FAILURE);
	if (is_builtin(&cmd->args[0]))
		exit(exec_builtin(cmd->args, shell));
	else
		exec_binary(shell, cmd->args);
}

int	execute_command(t_shell *shell, t_cmd *cmd, t_pipe_state state,
		int shared_pipe[2])
{
	t_pipe_info			pipe_info;
	pid_t				pid;
	struct sigaction	new_sa;
	struct sigaction	old_sa;

	init_pipe_info(&pipe_info, state, shared_pipe);
	create_pipe(state, pipe_info.new_pipe);
	if (state == NO_PIPE && is_builtin(&cmd->args[0]))
		return (exec_builtin_parent(shell, cmd, cmd->args));
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
		execute_in_child(shell, cmd, &pipe_info);
	setup_signal_handler(&new_sa, &old_sa);
	cleanup_pipe_ext(&pipe_info, shared_pipe);
	sigaction(SIGINT, &old_sa, NULL);
	cmd->pid = pid;
	return (EXIT_SUCCESS);
}
