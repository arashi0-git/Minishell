/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:26:46 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/09 15:44:06 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parse.h"
#include "../../include/redirect.h"
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
		free(line);
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}
