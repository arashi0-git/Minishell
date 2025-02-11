/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:49:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 15:49:14 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		RedirectType redirType)
{
	cmd->outfile = malloc(ft_strlen(target->value) + 1);
	if (cmd->outfile == NULL)
	{
		printf("malloc outfile failed\n");
		return (-1);
	}
	ft_strcpy(cmd->outfile, target->value);
	if (redirType == REDIRECT_APPEND)
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
		printf("Syntax error: redirection missing target\n");
		return (-1);
	}
	if (redir->redirType == REDIRECT_IN || redir->redirType == REDIRECT_HEREDOC)
	{
		ret = handle_input_redirection(cmd, *curr_ptr);
		if (ret != 0)
			return (ret);
	}
	else if (redir->redirType == REDIRECT_OUT
		|| redir->redirType == REDIRECT_APPEND)
	{
		ret = handle_output_redirection(cmd, *curr_ptr, redir->redirType);
		if (ret != 0)
			return (ret);
	}
	return (0);
}
