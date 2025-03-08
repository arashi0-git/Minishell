/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:41:00 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 21:19:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "redirect.h"

int	process_pipe_token(t_token *token, t_cmd **current_cmd)
{
	if (token->next == NULL)
	{
		printf("minishell: syntax error unexpected token '|' or '||'\n");
		return (-1);
	}
	*current_cmd = NULL;
	return (1);
}

int	process_redir_token(t_token *token, t_cmd *current_cmd)
{
	if (token->next == NULL)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	return (handle_redirection(current_cmd, token));
}

int	process_command_token(t_token *token, t_cmd *current_cmd)
{
	if (handle_command_token(current_cmd, token) != 0)
		return (-1);
	return (1);
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
