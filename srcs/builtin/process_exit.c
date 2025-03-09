/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:52:43 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/10 02:39:48 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_exit_message(char **args)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit(2);
}

int	exec_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (shell->interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	if (!args[1])
	{
		exit_code = shell->exit_status;
		exit(exit_code);
	}
	if (ft_isdigit(args[1][0]) || args[1][0] == '-' || args[1][0] == '+'
		|| (args[1][0] == ' ' && ft_isdigit(args[1][1])))
	{
		exit_code = ft_atoi(args[1]);
		if (exit_code == 0 && strcmp(args[1], "0") != 0)
			print_exit_message(args);
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		exit(exit_code);
	}
	print_exit_message(args);
	return (0);
}
