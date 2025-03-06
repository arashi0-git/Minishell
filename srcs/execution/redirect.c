/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/07 07:03:13 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

// Function prototype for the missing function
static void	print_bad_fd_error(int fd)
{
	char	fd_str[12];

	sprintf(fd_str, "%d", fd);
	print_error("Bad file descriptor", NULL, fd_str);
}

// 入力リダイレクト用のトークンとリダイレクト構造体を作成
static t_redirect	*create_in_redirect(t_cmd *command)
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

// 出力リダイレクト用のトークンとリダイレクト構造体を作成
static t_redirect	*create_out_redirect(t_cmd *command)
{
	t_token			*token;
	t_redirect		*redir;
	t_redirecttype	type;

	type = command->append ? REDIRECT_APPEND : REDIRECT_OUT;
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

// ヒアドキュメントの入力を読み込む関数
char	*read_until_delimiter(char *delimiter)
{
	char	*line;
	char	*content;

	// char *tmp;
	content = ft_strdup("");
	if (!content)
		return (NULL);
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break ;
		if (process_delimiter(line, delimiter, &content))
			break ;
		if (!append_to_content(&content, line))
			return (NULL);
	}
	return (content);
}

// 区切り文字のチェック処理を分離
int	process_delimiter(char *line, char *delimiter, char **content)
{
	char	*tmp;

	tmp = ft_strjoin(delimiter, "\n");
	if (!tmp)
	{
		free(line);
		free(*content);
		*content = NULL;
		return (1);
	}
	if (ft_strcmp(line, tmp) == 0)
	{
		free(line);
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

// 内容の追加処理を分離
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

// ヒアドキュメント処理関数
int	handle_heredoc(t_redirect *redir)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		print_error(strerror(errno), NULL, "heredoc");
		return (-1);
	}
	redir->here_doc_content = read_until_delimiter(redir->filename->value);
	if (!redir->here_doc_content)
	{
		close_pipe_fds(pipe_fd);
		return (-1);
	}
	ft_putstr_fd(redir->here_doc_content, pipe_fd[1]);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

// パイプのクローズ処理を分離
void	close_pipe_fds(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

static int	open_file(t_redirect *redir)
{
	char	*filename;

	if (redir->type == REDIRECT_HEREDOC)
		return (handle_heredoc(redir));
	filename = redir->filename->value;
	fprintf(stderr, "Debug: Attempting to open file '%s'\n", filename);
	fprintf(stderr, "Debug: Redirect type: %d\n", redir->type);
	filename = redir->filename->value;
	if (redir->type == REDIRECT_IN)
		return (open(filename, O_RDONLY));
	if (redir->type == REDIRECT_OUT)
		return (open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_MODE));
	return (open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_MODE));
}

// redirect checker
t_bool	check_redirect(t_redirect *redir)
{
	if (redir->filename == NULL || redir->filename->next)
	{
		print_error("ambiguous redirect", NULL, redir->filename->value);
		// Changed from data to value
		return (FALSE);
	}
	if ((redir->fd_file = open_file(redir)) < 0)
	{
		print_error(strerror(errno), NULL, redir->filename->value);
		// Changed from data to value
		return (FALSE);
	}
	// debug
	fprintf(stderr, "Debug: Successfully opened file '%s', fd_file = %d\n",
		redir->filename->value, redir->fd_file);
	return (TRUE);
}

// リダイレクトのsetUp

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
		fprintf(stderr, "Debug: In dup_redirects - type: %d, fd_io: %d, fd_file:
			%d\n", redir->type, redir->fd_io, redir->fd_file);
		if (is_parent)
		{
			if ((redir->fd_backup = dup(redir->fd_io)) < 0)
			{
				print_bad_fd_error(redir->fd_io);
				return (FALSE);
			}
			fprintf(stderr, "Debug: Backup fd created: fd_backup = %d\n",
				redir->fd_backup);
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
	redir->here_doc_content = NULL;
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
		if (redir->here_doc_content)
			free(redir->here_doc_content);
		free(redir);
		redir = next;
	}
}
