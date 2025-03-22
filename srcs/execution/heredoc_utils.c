/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/22 20:37:35 by marvin           ###   ########.fr       */
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
	int	original_signal;

	original_signal = g_signal;
	ft_putstr_fd("> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	if (g_signal != original_signal)
	{
		free(line);
		return (NULL);
	}
	expanded = expand(line, shell);
	free(line);
	if (g_signal != original_signal)
	{
		free(expanded);
		return (NULL);
	}
	return (expanded);
}
