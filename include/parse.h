/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:59:33 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:02:10 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

typedef struct s_token	t_token;
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
	int					append;
	struct s_cmd		*next;
	struct s_cmd *prev;           // パイプライン用
	pid_t pid;                    // プロセス管理用
	struct s_redirect *redirects; // リダイレクト管理用
}						t_cmd;

/*---parse func---*/
t_cmd					*parse_tokens(t_token *tokens);
int						handle_redirection(t_cmd *cmd, t_token **curr_ptr);
int						add_arg(t_cmd *cmd, const char *arg);
t_cmd					*new_cmd(void);

#endif
