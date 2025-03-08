/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:59:33 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 21:19:31 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "./minishell.h"
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
	int					append;
	struct s_cmd		*next;
	struct s_cmd		*prev;
	pid_t				pid;
	struct s_redirect	*redirects;
}						t_cmd;

/*---parse func---*/
t_cmd					*parse_tokens(t_token *tokens);
int						handle_redirection(t_cmd *cmd, t_token **curr_ptr);
int						add_arg(t_cmd *cmd, const char *arg);
t_cmd					*new_cmd(void);
int						process_command_token(t_token *token,
							t_cmd *current_cmd);
int						process_redir_token(t_token *token, t_cmd *current_cmd);
int						process_pipe_token(t_token *token, t_cmd **current_cmd);
int						handle_command_token(t_cmd *cmd, t_token *token);
void					add_redirect_to_list(t_cmd *cmd, t_redirect *new_redir);

#endif
