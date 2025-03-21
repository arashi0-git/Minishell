/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:31:20 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/22 19:59:59 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

# include "./minishell.h"
# include "./tokenize.h"
# include <errno.h>
# include <fcntl.h>
# include <string.h>
# define FILE_MODE 0644

// リダイレクト構造体
typedef struct s_redirect
{
	t_redirecttype		type;
	t_token				*filename;
	int					fd_io;
	int					fd_file;
	int					fd_backup;
	char				*here_doc_content;
	struct s_redirect	*next;
	struct s_redirect	*prev;
}						t_redirect;

t_bool					setup_redirects(t_cmd *command);
void					cleanup_redirects(t_cmd *command);
t_bool					dup_redirects(t_cmd *command, t_bool is_parent);
t_redirect				*create_redirect(t_redirecttype type, t_token *filename,
							int fd_io);
void					free_redirect(t_redirect *redir);

char					*read_until_delimiter(char *delimiter, t_shell *shell);
int						process_delimiter(char *line, char *delimiter,
							char **content);
int						append_to_content(char **content, char *line);
int						handle_heredoc(t_redirect *redir);
void					close_pipe_fds(int pipe_fd[2]);
t_bool					check_redirect(t_redirect *redir);
void					cleanup_redirects(t_cmd *command);
t_redirect				*create_in_redirect(t_cmd *command);
t_redirect				*create_out_redirect(t_cmd *command);
t_bool					process_all_heredocs(t_cmd *cmd_list, t_shell *shell);
t_bool					setup_heredoc_content(t_cmd *cmd, char *delimiter,
							t_shell *shell);
int						open_file(t_redirect *redir);
void					close_pipe_fds(int pipe_fd[2]);
char					*get_expanded_line(t_shell *shell);
t_bool					process_cmd_heredocs(t_cmd *cmd, int original_signal,
							t_shell *shell);
int						process_single_line(char *delimiter, t_shell *shell,
							int original_signal, char **content);

#endif