/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 17:06:24 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/09 17:17:11 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	expand_length(const char *str, t_shell *shell)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (process_quote(str, shell, &i, &len) < 0)
				return (-1);
		}
		else if (str[i] == '$')
		{
			if (process_dollar_length(str, shell, &i, &len) < 0)
				return (-1);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*perform_expansion(const char *str, t_shell *shell, size_t total_len)
{
	char	*out;
	size_t	out_index;
	size_t	i;

	out = malloc(total_len + 1);
	out_index = 0;
	i = 0;
	if (!out)
		return (NULL);
	while (str[i] != '\0')
	{
		if (process_expansion_char(str, shell, out, &i, &out_index) < 0)
		{
			free(out);
			return (NULL);
		}
	}
	out[out_index] = '\0';
	return (out);
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
