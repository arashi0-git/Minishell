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

t_redirect	*create_in_redirect(t_cmd *command)
{
	t_token		*token;
	t_redirect	*redir;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = command->infile;
	token->next = NULL;
	token->type = TOKEN_COMMAND;
	redir = create_redirect(REDIRECT_IN, token, STDIN_FILENO);
	if (!redir || check_redirect(redir) == FALSE)
	{
		if (redir)
			free_redirect(redir);
		else
			free(token);
		return (NULL);
	}
	return (redir);
}

t_redirect	*create_out_redirect(t_cmd *command)
{
	t_token			*token;
	t_redirect		*redir;
	t_redirecttype	type;

	if (command->append)
		type = REDIRECT_APPEND;
	else
		type = REDIRECT_OUT;
	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = command->outfile;
	token->next = NULL;
	token->type = TOKEN_COMMAND;
	redir = create_redirect(type, token, STDOUT_FILENO);
	if (!redir || check_redirect(redir) == FALSE)
	{
		if (redir)
			free_redirect(redir);
		else
			free(token);
		return (NULL);
	}
	return (redir);
}

t_bool	check_redirect(t_redirect *redir)
{
	int	file_fd;

	if (redir->filename == NULL || redir->filename->next)
	{
		print_error("ambiguous redirect", NULL, redir->filename->value);
		return (FALSE);
	}
	if (redir->type == REDIRECT_HEREDOC)
		return (TRUE);
	file_fd = open_file(redir);
	if (file_fd < 0)
	{
		print_error(strerror(errno), NULL, redir->filename->value);
		return (FALSE);
	}
	redir->fd_file = file_fd;
	return (TRUE);
}

t_redirect	*create_redirect(t_redirecttype type, t_token *filename, int fd_io)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		error_exit(NULL);
	redir->type = type;
	redir->filename = filename;
	redir->fd_io = fd_io;
	redir->fd_file = -1;
	redir->fd_backup = -1;
	redir->here_doc_content = NULL;
	redir->next = NULL;
	redir->prev = NULL;
	return (redir);
}
