/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:49:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:38:32 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "redirect.h"

extern t_shell	*g_shell;

static int	copy_filename(char **dest, const char *src)
{
	*dest = malloc(ft_strlen(src) + 1);
	if (!*dest)
	{
		printf("malloc filename failed\n");
		return (-1);
	}
	ft_strcpy(*dest, src);
	return (0);
}

// リダイレクトをリストの末尾に追加する関数
void	add_redirect_to_list(t_cmd *cmd, t_redirect *new_redir)
{
	t_redirect	*current;

	if (cmd->redirects == NULL)
	{
		cmd->redirects = new_redir;
		return ;
	}
	current = (t_redirect *)cmd->redirects;
	while (current->next)
	{
		current = current->next;
	}
	current->next = new_redir;
	new_redir->prev = current;
}

// 入力リダイレクション処理
static int	handle_input_redirection(t_cmd *cmd, t_token *target)
{
	t_redirect	*redir;

	if (copy_filename(&cmd->infile, target->value) < 0)
		return (-1);
	redir = create_redirect(REDIRECT_IN, target, STDIN_FILENO);
	if (!redir || !check_redirect(redir))
	{
		if (redir)
			free_redirect(redir);
		return (-1);
	}
	add_redirect_to_list(cmd, redir);
	// redir->next = (t_redirect *)cmd->redirects;
	// cmd->redirects = redir;
	return (0);
}

// 出力リダイレクション処理
static int	handle_output_redirection(t_cmd *cmd, t_token *target,
		t_redirecttype redirtype)
{
	t_redirect	*redir;

	if (copy_filename(&cmd->outfile, target->value) < 0)
		return (-1);
	cmd->append = (redirtype == REDIRECT_APPEND) ? 1 : 0;
	redir = create_redirect(redirtype, target, STDOUT_FILENO);
	if (!redir || !check_redirect(redir))
	{
		if (redir)
			free_redirect(redir);
		return (-1);
	}
	add_redirect_to_list(cmd, redir);
	// redir->next = (t_redirect *)cmd->redirects;
	// cmd->redirects = redir;
	return (0);
}

// トークン複製関数（新規追加）
static t_token	*duplicate_token(t_token *src)
{
	t_token	*new_token;
	char	*value_copy;

	value_copy = ft_strdup(src->value);
	if (!value_copy)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		free(value_copy);
		return (NULL);
	}
	new_token->value = value_copy;
	new_token->type = src->type;
	new_token->redirtype = src->redirtype;
	new_token->next = NULL;
	return (new_token);
}

// リダイレクト処理メイン関数
int	handle_redirection(t_cmd *cmd, t_token **curr_ptr)
{
	int		ret;
	t_token	*redir;
	t_token	*target_copy;

	redir = *curr_ptr;
	*curr_ptr = (*curr_ptr)->next;
	if (*curr_ptr == NULL || (*curr_ptr)->type != TOKEN_COMMAND)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_shell->exit_status = 2;
		return (-1);
	}
	target_copy = duplicate_token(*curr_ptr);
	if (!target_copy)
		return (-1);
	if (redir->redirtype == REDIRECT_IN || redir->redirtype == REDIRECT_HEREDOC)
		ret = handle_input_redirection(cmd, target_copy);
	else
		ret = handle_output_redirection(cmd, target_copy, redir->redirtype);
	if (ret != 0)
		free_token_list(target_copy);
	return (ret);
}
