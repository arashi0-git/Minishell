/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 11:55:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/09 19:57:14 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	minishell_pipe(t_shell *shell)
{
	char	*line;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (line[0] != '\0')
			process_input(shell, line);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
}
