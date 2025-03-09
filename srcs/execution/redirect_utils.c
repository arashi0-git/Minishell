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

// 入力リダイレクト用のトークンとリダイレクト構造体を作成

int	append_to_content(char **content, char *line)
{
	char	*tmp;

	tmp = *content;
	*content = ft_strjoin(*content, line);
	free(tmp);
	free(line);
	if (!*content)
		return (0);
	return (1);
}

void	print_bad_fd_error(int fd)
{
	char	fd_str[12];

	sprintf(fd_str, "%d", fd);
	print_error("Bad file descriptor", NULL, fd_str);
}

// リダイレクト構造体の解放
void	free_redirect(t_redirect *redir)
{
	t_redirect	*next;

	while (redir)
	{
		next = redir->next;
		if (redir->filename)
			free_token_list(redir->filename);
		if (redir->here_doc_content)
			free(redir->here_doc_content);
		free(redir);
		redir = next;
	}
}

void	close_pipe_fds(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

int	open_file(t_redirect *redir)
{
	char	*filename;

	if (redir->type == REDIRECT_HEREDOC)
		return (handle_heredoc(redir));
	filename = redir->filename->value;
	filename = redir->filename->value;
	if (redir->type == REDIRECT_IN)
		return (open(filename, O_RDONLY));
	if (redir->type == REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_MODE));
}
