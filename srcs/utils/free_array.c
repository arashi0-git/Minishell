/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_array.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 19:45:38 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/28 16:05:34 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

void	free_array(char **sorted_env)
{
	int	i;

	i = 0;
	if (!sorted_env)
		return ;
	while (sorted_env[i])
	{
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

void	cleanup_cmd_fds(t_cmd *cmd)
{
	t_redirect	*redir;

	if (!cmd || !cmd->redirects)
		return ;
	redir = cmd->redirects;
	while (redir)
	{
		if (redir->fd_file > 2)
			close(redir->fd_file);
		if (redir->fd_backup > 2)
			close(redir->fd_backup);
		redir = redir->next;
	}
}

void	cleanup_all_fds(t_cmd *cmd_list)
{
	t_cmd	*cmd;

	if (!cmd_list)
		return ;
	cmd = cmd_list;
	while (cmd)
	{
		cleanup_cmd_fds(cmd);
		cmd = cmd->next;
	}
}
