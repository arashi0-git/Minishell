/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:27:41 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 11:52:51 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/minishell.h"
#include "../../include/parse.h"
#include <sys/wait.h>

// void	print_cmd_list(t_cmd *cmd_list)
// {
// 	int	i;
// 	int	j;

// 	i = 0;
// 	while (cmd_list)
// 	{
// 		j = 0;
// 		printf("Command %d:\n", i);
// 		if (cmd_list->command)
// 			printf("  Command: %s\n", cmd_list->command);
// 		if (cmd_list->argc > 0)
// 		{
// 			while (j < cmd_list->argc)
// 			{
// 				printf("  Arg[%d]: %s\n", j, cmd_list->args[j]);
// 				j++;
// 			}
// 		}
// 		if (cmd_list->infile)
// 			printf(" Infile: %s\n", cmd_list->infile);
// 		if (cmd_list->outfile)
// 			printf(" Outfile: %s(%s)\n", cmd_list->outfile,
// 				(cmd_list->append ? "append" : "overwrite"));
// 		cmd_list = cmd_list->next;
// 		i++;
// 	}
// }

void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*tmp;
	int		i;

	i = 0;
	while (cmd_list)
	{
		tmp = cmd_list;
		cmd_list = cmd_list->next;
		if (tmp->args)
		{
			i = 0;
			while (i < tmp->argc)
				free(tmp->args[i++]);
			free(tmp->args);
		}
		if (tmp->command)
			free(tmp->command);
		if (tmp->infile)
			free(tmp->infile);
		if (tmp->outfile)
			free(tmp->outfile);
		free(tmp);
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

t_cmd	*tokenize_and_parse(char *input)
{
	t_token	*token_list;
	t_cmd	*cmd_list;

	token_list = tokenize_list(input);
	if (!token_list)
	{
		printf("Tokenization error\n");
		return (NULL);
	}
	cmd_list = parse_tokens(token_list);
	if (!cmd_list)
	{
		printf("Parsing error\n");
		free_token_list(token_list);
		return (NULL);
	}
	free_token_list(token_list);
	return (cmd_list);
}

void	process_output(t_shell *shell, t_cmd *cmd_list)
{
	t_cmd			*cmd;
	int				status;
	t_pipe_state	state;
	pid_t			last_pid;
	int				pipeline_pipe[2] = {-1, -1};

	cmd = cmd_list;
	last_pid = -1;
	while (cmd != NULL)
	{
		init_pipe_state(&state, cmd);
		status = execute_command(shell, cmd, state, pipeline_pipe);
		if (state == PIPE_READ_ONLY)
			last_pid = cmd->pid;
		cmd = cmd->next;
	}
	if (cmd_list && cmd_list->next && last_pid > 0)
	{
		if (waitpid(last_pid, NULL, WNOHANG) == 0)
		{
			status = wait_for_command(last_pid);
			shell->exit_status = status;
		}
	}
}

void	process_input(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	t_cmd	*cmd;

	cmd_list = tokenize_and_parse(input);
	if (!cmd_list)
	{
		free_cmd_list(cmd_list);
		return ;
	}
	cmd = cmd_list;
	while (cmd != NULL)
	{
		expand_cmd(cmd, shell);
		cmd = cmd->next;
	}
	// print_cmd_list(cmd_list);
	process_output(shell, cmd_list);
	free_cmd_list(cmd_list);
}
