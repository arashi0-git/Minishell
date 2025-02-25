/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 16:58:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/25 16:25:11 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/../../include/minishell.h"

void	print_error(char *message, char *command)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(message, STDERR_FILENO);
}

void	error_exit(char *command)
{
	print_error(strerror(errno), command);
	exit(EXIT_FAILURE);
}

