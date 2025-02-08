/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:06:24 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/08 17:48:38 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_cmd(t_cmd *cmd, t_shell *shell)
{
	int		i;
	char	*expanded;

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
		if (expanded)
		{
			free(cmd->args[i]);
			cmd->args[i] = expanded;
		}
		i++;
	}
}
