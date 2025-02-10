/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:27:41 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 14:34:05 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	process_input(t_shell *shell, char *input)
{
	t_token	*token_list;
	t_cmd	*cmd_list;
	t_cmd	*cmd;

	token_list = tokenize_list(input);
	if (!token_list)
	{
		printf("Tokenization error\n");
		return ;
	}
	cmd_list = parse_tokens(token_list);
	if (!cmd_list)
	{
		printf("Parsing error\n");
		// free_token_list(token_list);
		return ;
	}
	cmd = cmd_list;
	while (cmd != NULL)
	{
		expand_cmd(cmd, shell);
		cmd = cmd->next;
	}
	// execute_cmd(cmd_list, shell);
	// free_cmd_list(cmd_list);
	// free_token_list(token_list);
}
