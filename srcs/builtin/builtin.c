/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:32 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 19:12:47 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/minishell.h"

int	exec_builtin(char **args, t_shell *shell)
{
	if (ft_strcmp(args[0], "exit") == 0)
		return (exec_exit(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (exec_cd(args, shell));
	if (ft_strcmp(args[0], "echo") == 0)
		return (exec_echo(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (exec_pwd());
	if (ft_strcmp(args[0], "env") == 0)
		return (exec_env(args, shell));
	if (ft_strcmp(args[0], "export") == 0)
		return (exec_export(args, shell));
	if (ft_strcmp(args[0], "unset") == 0)
		return (exec_unset(args, shell));
	return (EXIT_FAILURE);
}

int	is_builtin(char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (1);
	if (ft_strcmp(args[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(args[0], "env") == 0)
		return (1);
	if (ft_strcmp(args[0], "export") == 0)
		return (1);
	if (ft_strcmp(args[0], "unset") == 0)
		return (1);
	return (0);
}
