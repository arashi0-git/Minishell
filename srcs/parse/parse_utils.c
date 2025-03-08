/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:41:00 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 20:41:53 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"

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
	return (handle_redirection(current_cmd, token, token->next));
}

int	process_command_token(t_token *token, t_cmd *current_cmd)
{
	if (handle_command_token(current_cmd, token) != 0)
		return (-1);
	return (1);
}
