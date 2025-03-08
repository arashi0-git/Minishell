/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:42:53 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 22:48:13 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

t_cmd	*create_new_command(t_cmd **cmd_list, t_cmd **current_cmd)
{
	t_cmd	*new_cmd_ptr;
	t_cmd	*tail;

	new_cmd_ptr = new_cmd();
	if (!*cmd_list)
		*cmd_list = new_cmd_ptr;
	else
	{
		tail = *cmd_list;
		while (tail->next)
			tail = tail->next;
		tail->next = new_cmd_ptr;
		new_cmd_ptr->prev = tail;
	}
	*current_cmd = new_cmd_ptr;
	return (new_cmd_ptr);
}

int	handle_command_token(t_cmd *cmd, t_token *token)
{
	fprintf(stderr, "Adding token '%s' as command/arg\n", token->value);
	if (!cmd->command)
	{
		cmd->command = malloc(ft_strlen(token->value) + 1);
		if (!cmd->command)
		{
			printf("malloc command failed\n");
			return (-1);
		}
		ft_strcpy(cmd->command, token->value);
		if (add_arg(cmd, token->value) != 0)
		{
			printf("add_arg failed\n");
			return (-1);
		}
	}
	else
	{
		if (add_arg(cmd, token->value) != 0)
		{
			printf("add_arg failed\n");
			return (-1);
		}
	}
	return (0);
}

static int	process_token(t_token **curr_ptr, t_cmd **cmd_list,
		t_cmd **current_cmd)
{
	if (!*current_cmd && (*curr_ptr)->type == TOKEN_PIPE)
	{
		printf("minishell: syntax error unexpected token '|'or '||'\n");
		return (-1);
	}
	if (!*current_cmd)
		create_new_command(cmd_list, current_cmd);
	if ((*curr_ptr)->type == TOKEN_PIPE)
	{
		if ((*curr_ptr)->next == NULL)
			return (-1);
		*current_cmd = NULL;
	}
	else if ((*curr_ptr)->type == TOKEN_REDIR)
	{
		if (handle_redirection(*current_cmd, curr_ptr) != 0)
			return (-1);
	}
	else if ((*curr_ptr)->type == TOKEN_COMMAND)
	{
		if (handle_command_token(*current_cmd, *curr_ptr) != 0)
			return (-1);
	}
	return (0);
}

t_cmd	*parse_tokens(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;
	t_token	*curr;

	cmd_list = NULL;
	current_cmd = NULL;
	curr = tokens;
	while (curr)
	{
		if (process_token(&curr, &cmd_list, &current_cmd) != 0)
		{
			free_cmd_list(cmd_list);
			return (NULL);
		}
		curr = curr->next;
	}
	return (cmd_list);
}
