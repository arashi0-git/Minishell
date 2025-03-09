/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:31:20 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:36:36 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "./minishell.h"
# include "redirect.h"
# include <errno.h>
# include <libft.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <tokenize.h>
# include <unistd.h>

typedef struct s_token	t_token;
/*---parser struct---*/

typedef struct s_cmd
{
	char				*command;
	char				**args;
	int					argc;
	int					max_args;
	char				*infile;
	char				*outfile;
	int					heredoc_flag;
	t_list*				heredoc_delims;
	int					append;
	struct s_cmd		*next;
	struct s_cmd		*prev;
	pid_t				pid;
	struct s_redirect	*redirects;
}						t_cmd;

/*---parse func---*/
t_cmd					*parse_tokens(t_token *tokens, t_shell *shell);
int						handle_redirection(t_cmd *cmd, t_token **curr_ptr,
							t_shell *shell);
int						add_arg(t_cmd *cmd, const char *arg);
t_cmd					*new_cmd(void);
void					add_redirect_to_list(t_cmd *cmd, t_redirect *new_redir);
int						process_io_redirection(t_cmd *cmd, t_token *redir,
							t_token *target_copy);
int						check_and_dup_token(t_token **curr_ptr, t_shell *shell,
							t_token **dup_token);
int						copy_filename(char **dest, const char *src);
int						handle_input_redirection(t_cmd *cmd, t_token *target);
int						handle_output_redirection(t_cmd *cmd, t_token *target,
							t_redirecttype redirtype);
t_token					*duplicate_token(t_token *src);

#endif
