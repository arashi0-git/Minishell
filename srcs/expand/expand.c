/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:06:24 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/09 21:25:18 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

t_expand	*init_expand(size_t total_len)
{
	t_expand	*exp;

	exp = malloc(sizeof(t_expand));
	if (!exp)
		return (NULL);
	exp->out = malloc(total_len + 1);
	if (!exp->out)
	{
		free(exp);
		return (NULL);
	}
	exp->i = 0;
	exp->out_index = 0;
	exp->max = total_len;
	return (exp);
}

int	expand_length(const char *str, t_shell *shell)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	if (!str || (uintptr_t)str < 0x1000)
	{
		return (0);
	}
	if (str[0] == '\0')
	{
		return (0);
	}
	while (str[i] != '\0')
	{
		if (process_character(str, shell, &i, &len) < 0)
			return (-1);
	}
	return (len);
}

char	*expand(const char *str, t_shell *shell)
{
	int	total_len;

	total_len = expand_length(str, shell);
	if (total_len == -1)
	{
		shell->exit_status = 127;
		return (NULL);
	}
	return (perform_expansion(str, shell, total_len));
}

void	expand_redirects(t_cmd *cmd, t_shell *shell)
{
	char	*expanded;

	if (cmd->infile && !cmd->heredoc_flag)
	{
		expanded = expand(cmd->infile, shell);
		if (expanded)
		{
			free(cmd->infile);
			cmd->infile = expanded;
		}
	}
	if (cmd->outfile)
	{
		expanded = expand(cmd->outfile, shell);
		if (expanded)
		{
			free(cmd->outfile);
			cmd->outfile = expanded;
		}
	}
}

void	expand_cmd(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

	if (!cmd->command)
		return ;
	if (cmd->command)
	{
		expanded = expand(cmd->command, shell);
		if (expanded)
		{
			free(cmd->command);
			cmd->command = expanded;
		}
	}
	i = 0;
	while (cmd->args && cmd->args[i])
	{
		expanded = expand(cmd->args[i], shell);
		if (expanded && expanded != cmd->args[i])
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}
