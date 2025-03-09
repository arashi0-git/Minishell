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

static t_bool	backup_redirects(t_redirect *redir)
{
	int	backup_fd;

	while (redir)
	{
		backup_fd = dup(redir->fd_io);
		if (backup_fd < 0)
		{
			print_bad_fd_error(redir->fd_io);
			return (FALSE);
		}
		redir->fd_backup = backup_fd;
		redir = redir->next;
	}
	return (TRUE);
}

static t_bool	apply_redirects(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == REDIRECT_HEREDOC)
		{
			redir->fd_file = handle_heredoc(redir);
			if (redir->fd_file < 0)
				return (FALSE);
		}
		if (dup2(redir->fd_file, redir->fd_io) < 0)
		{
			print_bad_fd_error(redir->fd_io);
			return (FALSE);
		}
		redir = redir->next;
	}
	return (TRUE);
}

t_bool	dup_redirects(t_cmd *command, t_bool is_parent)
{
	if (is_parent && !backup_redirects((t_redirect *)command->redirects))
		return (FALSE);
	return (apply_redirects((t_redirect *)command->redirects));
}

static void	link_redirects(t_cmd *command, t_redirect *in_redir,
	t_redirect *out_redir)
{
	if (in_redir && out_redir)
	{
		in_redir->next = out_redir;
		out_redir->prev = in_redir;
		command->redirects = in_redir;
	}
	else if (in_redir)
		command->redirects = in_redir;
	else if (out_redir)
		command->redirects = out_redir;
}

t_bool	setup_redirects(t_cmd *command)
{
	t_redirect	*in_redir;
	t_redirect	*out_redir;

	in_redir = NULL;
	out_redir = NULL;
	if (command->redirects)
		return (TRUE);
	if (command->infile)
		in_redir = create_in_redirect(command);
	if (command->outfile)
		out_redir = create_out_redirect(command);
	if ((command->infile && !in_redir) || (command->outfile && !out_redir))
	{
		if (in_redir)
			free_redirect(in_redir);
		if (out_redir)
			free_redirect(out_redir);
		return (FALSE);
	}
	link_redirects(command, in_redir, out_redir);
	return (TRUE);
}
