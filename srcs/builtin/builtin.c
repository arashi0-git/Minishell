/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:32 by aryamamo          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/25 15:51:34 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/bulitin.h"
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
=======
/*   Updated: 2025/02/28 16:40:44 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

// int	exec_builtin(char **args, t_shell *shell)
// {
// 	if (ft_strcmp(args[0], "exit") == 0)
// 		return (exec_exit(args));
// 	if (ft_strcmp(args[0], "cd") == 0)
// 		return (exec_cd(args, shell));
// 	if (ft_strcmp(args[0], "echo") == 0)
// 		return (exec_echo(args));
// 	if (ft_strcmp(args[0], "pwd") == 0)
// 		return (exec_pwd(args));
// 	if (ft_strcmp(args[0], "env") == 0)
// 		return (exec_env(args, shell));
// 	if (ft_strcmp(args[0], "export") == 0)
// 		return (exec_export(args, shell));
// 	if (ft_strcmp(args[0], "unset") == 0)
// 		return (exec_unset(args, shell));
// 	return (EXIT_FAILURE);
// }
>>>>>>> 49e45061e1b36f1b375e40f4c74b7bc66b6c7936

int	is_builtin(char **args)
{
	const char	*commands[] = {"exit", "cd", "pwd", "env", "export", "unset",
			"echo", NULL};
	int			i;

	i = 0;
	if (args[0] == NULL)
		return (0);
	while (commands[i])
	{
		if (ft_strcmp(args[0], (char *)commands[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}
