/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_handle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:49:25 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/09 22:19:46 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "redirect.h"

int	handle_input_redirection(t_cmd *cmd, t_token *target)
{
	t_redirect	*redir;

	if (copy_filename(&cmd->infile, target->value) < 0)
		return (-1);
	redir = create_redirect(REDIRECT_IN, target, STDIN_FILENO);
	if (!redir || !check_redirect(redir))
	{
		if (redir)
			free_redirect(redir);
		else
			free_token_list(target);
		return (-1);
	}
	add_redirect_to_list(cmd, redir);
	return (0);
}

int	handle_output_redirection(t_cmd *cmd, t_token *target,
		t_redirecttype redirtype)
{
	t_redirect	*redir;

	if (copy_filename(&cmd->outfile, target->value) < 0)
		return (-1);
	if (redirtype == REDIRECT_APPEND)
		cmd->append = 1;
	else
		cmd->append = 0;
	redir = create_redirect(redirtype, target, STDOUT_FILENO);
	if (!redir || !check_redirect(redir))
	{
		if (redir)
			free_redirect(redir);
		else
			free_token_list(target);
		return (-1);
	}
	add_redirect_to_list(cmd, redir);
	return (0);
}

static int	handle_heredoc_redirection(t_cmd *cmd, t_token *target)
{
	t_redirect	*redir;
	char		*delim_copy;
	t_list		*new_node;

	if (copy_filename(&cmd->infile, target->value) < 0)
		return (-1);
	delim_copy = ft_strdup(target->value);
	if (!delim_copy)
		return (-1);
	new_node = ft_lstnew(delim_copy);
	if (!new_node)
	{
		free(delim_copy);
		return (-1);
	}
	ft_lstadd_back(&cmd->heredoc_delims, new_node);
	redir = create_redirect(REDIRECT_HEREDOC, target, STDIN_FILENO);
	if (!redir)
		return (-1);
	add_redirect_to_list(cmd, redir);
	return (0);
}

t_token	*duplicate_token(t_token *src)
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

int	handle_redirection(t_cmd *cmd, t_token **curr_ptr, t_shell *shell)
{
	int		ret;
	t_token	*redir;
	t_token	*target_copy;

	redir = *curr_ptr;
	*curr_ptr = (*curr_ptr)->next;
	if (check_and_dup_token(curr_ptr, shell, &target_copy) < 0)
		return (-1);
	if (redir->redirtype == REDIRECT_HEREDOC)
	{
		cmd->heredoc_flag = 1;
		ret = handle_heredoc_redirection(cmd, target_copy);
		if (ret != 0)
			free_token_list(target_copy);
		return (ret);
	}
	ret = process_io_redirection(cmd, redir, target_copy);
	return (ret);
}
