/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:49:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 20:38:03 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "redirect.h"

static int	handle_input_redirection(t_cmd *cmd, t_token *target)
{
	t_redirect	*redir;

	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = strdup(target->value);
	if (!cmd->infile)
	{
		perror("strdup infile failed");
		return (-1);
	}
	redir = create_redirect(REDIRECT_IN, target, STDIN_FILENO);
	if (!redir)
		return (-1);
	if (!check_redirect(redir))
	{
		free_redirect(redir);
		return (-1);
	}
	redir->next = cmd->redirects;
	if (cmd->redirects)
		cmd->redirects->prev = redir;
	cmd->redirects = redir;
	return (0);
}

static int	handle_output_redirection(t_cmd *cmd, t_token *target,
		t_redirecttype redirtype)
{
	t_redirect	*redir;

	if (cmd->outfile)
		free(cmd->outfile);
	cmd->outfile = strdup(target->value);
	if (!cmd->outfile)
	{
		perror("strdup outfile failed");
		return (-1);
	}
	if (redirtype == REDIRECT_APPEND)
		cmd->append = 1;
	else
		cmd->append = 0;
	redir = create_redirect(redirtype, target, STDOUT_FILENO);
	if (!redir)
		return (-1);
	if (!check_redirect(redir))
	{
		free_redirect(redir);
		return (-1);
	}
	redir->next = cmd->redirects;
	if (cmd->redirects)
		cmd->redirects->prev = redir;
	cmd->redirects = redir;
	return (0);
}

int	handle_redirection(t_cmd *cmd, t_token *redir_token, t_token *file_token)
{
	int	ret;

	if (file_token == NULL || file_token->type != TOKEN_COMMAND)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return (-1);
	}
	if (redir_token->redirtype == REDIRECT_IN
		|| redir_token->redirtype == REDIRECT_HEREDOC)
	{
		ret = handle_input_redirection(cmd, file_token);
		if (ret != 0)
			return (ret);
	}
	else if (redir_token->redirtype == REDIRECT_OUT
		|| redir_token->redirtype == REDIRECT_APPEND)
	{
		ret = handle_output_redirection(cmd, file_token,
				redir_token->redirtype);
		if (ret != 0)
			return (ret);
	}
	return (2);
}
