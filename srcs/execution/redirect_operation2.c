/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/09 15:44:06 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

void	cleanup_redirects(t_cmd *command)
{
	t_redirect	*redir;

	redir = (t_redirect *)command->redirects;
	while (redir && redir->next)
		redir = redir->next;
	while (redir)
	{
		if (redir->fd_file >= 0)
		{
			if (close(redir->fd_file) < 0)
				error_exit(NULL);
		}
		if (redir->fd_backup >= 0)
		{
			if (dup2(redir->fd_backup, redir->fd_io) < 0
				|| close(redir->fd_backup) < 0)
			{
				error_exit(NULL);
			}
		}
		redir = redir->prev;
	}
}
