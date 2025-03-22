/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:27:41 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/23 01:11:03 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/parse.h"
#include <sys/wait.h>

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		free_cmd(tmp);
	}
}

void	free_token_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}

t_cmd	*tokenize_and_parse(char *input, t_shell *shell)
{
	t_token	*token_list;
	t_cmd	*cmd_list;
	int		i;

	i = 0;
	while (input[i])
	{
		if (ft_isascii(input[i]) == 0)
		{
			printf("minishell: syntax error near unexpected token\n");
			exit(1);
		}
		i++;
	}
	token_list = tokenize_list(input);
	if (!token_list)
		return (NULL);
	cmd_list = parse_tokens(token_list, shell);
	if (!cmd_list)
	{
		free_token_list(token_list);
		return (NULL);
	}
	free_token_list(token_list);
	return (cmd_list);
}

void	process_output(t_shell *shell, t_cmd *cmd_list)
{
	pid_t	last_pid;

	execute_commands(shell, cmd_list, &last_pid);
	wait_last_command(shell, cmd_list, last_pid);
}

void	process_input(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	t_cmd	*cmd;

	cmd_list = tokenize_and_parse(input, shell);
	if (!cmd_list)
	{
		free_cmd_list(cmd_list);
		return ;
	}
	cmd = cmd_list;
	while (cmd != NULL)
	{
		expand_cmd(cmd, shell);
		expand_redirects(cmd, shell);
		cmd = cmd->next;
	}
	if (!process_all_heredocs(cmd_list, shell))
	{
		free_cmd_list(cmd_list);
		return ;
	}
	process_output(shell, cmd_list);
	cleanup_all_fds(cmd_list);
	free_cmd_list(cmd_list);
}
