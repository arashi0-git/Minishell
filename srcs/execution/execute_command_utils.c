/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/10 05:26:22 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"

char	*generate_env_line(t_env *env)
{
	char	*env_line;
	char	*tmp;

	env_line = ft_strjoin(env->key, "=");
	if (!env_line)
		error_exit(NULL);
	tmp = env_line;
	env_line = ft_strjoin(env_line, env->value);
	if (!env_line)
		error_exit(NULL);
	free(tmp);
	return (env_line);
}

void	cleanup_pipe_ext(t_pipe_info *pipe_info, int shared_pipe[2])
{
	if (pipe_info->state == PIPE_READ_ONLY
		|| pipe_info->state == PIPE_READ_WRITE)
	{
		if (shared_pipe[PIPE_IN] != -1)
			close(shared_pipe[PIPE_IN]);
		if (shared_pipe[PIPE_OUT] != -1)
			close(shared_pipe[PIPE_OUT]);
	}
	if (pipe_info->state == PIPE_WRITE_ONLY
		|| pipe_info->state == PIPE_READ_WRITE)
	{
		if (pipe_info->new_pipe[PIPE_OUT] != -1)
			close(pipe_info->new_pipe[PIPE_OUT]);
		shared_pipe[PIPE_IN] = pipe_info->new_pipe[PIPE_IN];
		shared_pipe[PIPE_OUT] = -1;
	}
}

void	reset_signal_in_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
	sa.sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
}

void	init_pipe_info(t_pipe_info *pipe_info, t_pipe_state state,
		int shared_pipe[2])
{
	pipe_info->state = state;
	pipe_info->new_pipe[0] = -1;
	pipe_info->new_pipe[1] = -1;
	pipe_info->old_pipe[0] = shared_pipe[0];
	pipe_info->old_pipe[1] = shared_pipe[1];
}

void	setup_signal_handler(struct sigaction *new_sa, struct sigaction *old_sa)
{
	new_sa->sa_handler = SIG_IGN;
	sigemptyset(&new_sa->sa_mask);
	new_sa->sa_flags = 0;
	sigaction(SIGINT, new_sa, old_sa);
}
