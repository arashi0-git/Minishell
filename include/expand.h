/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:05:35 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:27:52 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

typedef struct s_shell	t_shell;
typedef struct s_cmd	t_cmd;
# include "./minishell.h"
# include <errno.h>
# include <libft.h>
# include <limits.h>
# include <parse.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <tokenize.h>
# include <unistd.h>
/*---expand struct---*/
typedef struct s_expand
{
	char				*out;
	size_t				i;
	size_t				out_index;
}						t_expand;

/*---expand func---*/
void					expand_cmd(t_cmd *cmd, t_shell *shell);
void					expand_redirects(t_cmd *cmd, t_shell *shell);
int						process_dollar_others(const char *str, size_t *i,
							size_t *len);
int						process_dollar_question(t_shell *shell, size_t *i,
							size_t *len);
int						process_quote_expand(const char *str, t_shell *shell,
							size_t *i, size_t *len);
int						process_dollar_length(const char *str, t_shell *shell,
							size_t *i, size_t *len);
int						process_expansion_char(const char *str, t_shell *shell,
							t_expand *exp);
t_expand				*init_expand(size_t total_len);
int						expand_dollar_question(t_shell *shell, t_expand *exp);
int						expand_dollar_variable(const char *str, t_expand *exp);

#endif
