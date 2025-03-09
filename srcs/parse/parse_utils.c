/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 20:18:05 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/10 08:05:43 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parse.h"
#include "redirect.h"

int	copy_filename(char **dest, const char *value)
{
	if (*dest)
	{
		free(*dest);
		*dest = NULL;
	}
	*dest = ft_strdup(value);
	if (!*dest)
	{
		printf("malloc dest failed\n");
		return (-1);
	}
	return (0);
}

int	check_and_dup_token(t_token **curr_ptr, t_shell *shell, t_token **dup_token)
{
	if (*curr_ptr == NULL || (*curr_ptr)->type != TOKEN_COMMAND)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		shell->exit_status = 2;
		return (-1);
	}
	*dup_token = duplicate_token(*curr_ptr);
	if (!(*dup_token))
		return (-1);
	return (0);
}

int	process_io_redirection(t_cmd *cmd, t_token *redir, t_token *target_copy)
{
	int	ret;

	if (redir->redirtype == REDIRECT_IN)
		ret = handle_input_redirection(cmd, target_copy);
	else
		ret = handle_output_redirection(cmd, target_copy, redir->redirtype);
	return (ret);
}
