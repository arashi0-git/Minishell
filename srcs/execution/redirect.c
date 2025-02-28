/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/28 15:54:54 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

// Function prototype for the missing function
static void	print_bad_fd_error(int fd)
{
	char	fd_str[12];

	sprintf(fd_str, "%d", fd);
	print_error("Bad file descriptor", fd_str);
}

static int	open_file(t_redirect *redir)
{
	char	*filename;

	filename = redir->filename->value; // Changed from data to value
	if (redir->type == REDIRECT_IN)
		return (open(filename, O_RDONLY));
	if (redir->type == REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_MODE));
}

// redirect checker
static t_bool	check_redirect(t_redirect *redir)
{
	if (redir->filename == NULL || redir->filename->next)
	{
		print_error("ambiguous redirect", redir->filename->value);
			// Changed from data to value
		return (FALSE);
	}
	if ((redir->fd_file = open_file(redir)) < 0)
	{
		print_error(strerror(errno), redir->filename->value);
			// Changed from data to value
		return (FALSE);
	}
	return (TRUE);
}

// リダイレクトのsetUp
t_bool	setup_redirects(t_cmd *command)
{
	t_redirect	*redir;

	// command->redirectsはt_redirect*型に変更する必要があります
	// この関数を呼び出す前に、コマンド構造体内のredirectsフィールドが
	// 正しく初期化されていることを確認してください
	redir = (t_redirect *)command->redirects; // キャスト追加
	while (redir)
	{
		if (check_redirect(redir) == FALSE)
		{
			cleanup_redirects(command);
			return (FALSE);
		}
		redir = redir->next;
	}
	return (TRUE);
}

// リダイレクトのcleanup
void	cleanup_redirects(t_cmd *command)
{
	t_redirect	*redir;

	redir = (t_redirect *)command->redirects; // キャスト追加
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

// リダイレクトの複製処理
t_bool	dup_redirects(t_cmd *command, t_bool is_parent)
{
	t_redirect	*redir;

	redir = (t_redirect *)command->redirects; // キャスト追加
	while (redir)
	{
		if (is_parent)
		{
			if ((redir->fd_backup = dup(redir->fd_io)) < 0)
			{
				print_bad_fd_error(redir->fd_io);
				return (FALSE);
			}
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

// リダイレクト構造体の作成
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
	redir->next = NULL;
	redir->prev = NULL;
	return (redir);
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
		free(redir);
		redir = next;
	}
}
