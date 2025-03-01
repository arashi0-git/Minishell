/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:49:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/28 16:21:06 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

extern t_shell	*g_shell;

static int	handle_input_redirection(t_cmd *cmd, t_token *target)
{
	cmd->infile = malloc(ft_strlen(target->value) + 1);
	if (!cmd->infile)
	{
		printf("malloc infile failed\n");
		return (-1);
	}
	ft_strcpy(cmd->infile, target->value);
	return (0);
}

static int	handle_output_redirection(t_cmd *cmd, t_token *target,
		t_redirecttype redirtype)
{
	cmd->outfile = malloc(ft_strlen(target->value) + 1);
	if (cmd->outfile == NULL)
	{
		printf("malloc outfile failed\n");
		return (-1);
	}
	ft_strcpy(cmd->outfile, target->value);
	if (redirtype == REDIRECT_APPEND)
		cmd->append = 1;
	else
		cmd->append = 0;
	return (0);
}

int	handle_redirection(t_cmd *cmd, t_token **curr_ptr)
{
	int		ret;
	t_token	*redir;

	redir = *curr_ptr;
	*curr_ptr = (*curr_ptr)->next;
	if (*curr_ptr == NULL || (*curr_ptr)->type != TOKEN_COMMAND)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		g_shell->exit_status = 2;
		return (-1);
	}
	if (redir->redirtype == REDIRECT_IN || redir->redirtype == REDIRECT_HEREDOC)
	{
		ret = handle_input_redirection(cmd, *curr_ptr);
		if (ret != 0)
			return (ret);
	}
	else if (redir->redirtype == REDIRECT_OUT
		|| redir->redirtype == REDIRECT_APPEND)
	{
		ret = handle_output_redirection(cmd, *curr_ptr, redir->redirtype);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
