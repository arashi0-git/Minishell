/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_add_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 13:51:02 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/11 12:04:22 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*new_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		printf("malloc cmd failed\n");
		return (NULL);
	}
	cmd->command = NULL;
	cmd->max_args = 4;
	cmd->args = malloc(sizeof(char *) * cmd->max_args);
	if (!cmd->args)
	{
		printf("malloc args failed\n");
		free(cmd);
		return (NULL);
	}
	cmd->argc = 0;
	cmd->outfile = NULL;
	cmd->infile = NULL;
	cmd->append = 0;
	cmd->next = NULL;
	return (cmd);
}

static int	resize_args(t_cmd *cmd)
{
	int		i;
	int		new_size;
	char	**new_args;

	i = 0;
	new_size = cmd->max_args + 2;
	new_args = malloc(sizeof(char *) * new_size);
	if (new_args == NULL)
	{
		printf("malloc new_args failed\n");
		return (-1);
	}
	while (i < cmd->argc)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[cmd->argc] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->max_args = new_size;
	return (0);
}

static int	copy_arg_to_cmd(t_cmd *cmd, const char *arg)
{
	cmd->args[cmd->argc] = malloc(ft_strlen(arg) + 1);
	if (cmd->args[cmd->argc] == NULL)
	{
		printf("malloc for arg copy failed\n");
		return (-1);
	}
	ft_strcpy(cmd->args[cmd->argc], arg);
	cmd->argc++;
	cmd->args[cmd->argc] = NULL;
	return (0);
}

int	add_arg(t_cmd *cmd, const char *arg)
{
	if (cmd->argc + 1 >= cmd->max_args)
	{
		if (resize_args(cmd) != 0)
			return (-1);
	}
	return (copy_arg_to_cmd(cmd, arg));
}
