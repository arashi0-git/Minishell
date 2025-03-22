/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/22 19:51:29 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
#include "expand.h"
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int	process_delimiter(char *line, char *delimiter, char **content)
{
	char	*tmp;

	tmp = ft_strjoin(delimiter, "\n");
	if (!tmp)
	{
		free(line);
		free(*content);
		*content = NULL;
		return (1);
	}
	if (ft_strcmp(line, tmp) == 0)
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}

char	*get_expanded_line(t_shell *shell)
{
	char	*line;
	char	*expanded;

	ft_putstr_fd("> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	expanded = expand(line, shell);
	free(line);
	return (expanded);
}

t_bool	process_cmd_heredocs(t_cmd *cmd, int original_signal, t_shell *shell)
{
	t_list	*delim_node;

	delim_node = cmd->heredoc_delims;
	while (delim_node)
	{
		if (!setup_heredoc_content(cmd, (char *)delim_node->content, shell))
		{
			if (g_signal != original_signal)
				return (FALSE);
			return (FALSE);
		}
		delim_node = delim_node->next;
	}
	return (TRUE);
}

int	process_single_line(char *delimiter, t_shell *shell, int original_signal,
		char **content)
{
	char	*line;

	line = get_expanded_line(shell);
	if (!line || g_signal != original_signal)
		return (0);
	if (process_delimiter(line, delimiter, content))
	{
		free(line);
		return (2);
	}
	if (!append_to_content(content, line))
		return (0);
	return (1);
}
