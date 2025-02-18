/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:14 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/11 11:36:38 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	exec_cd(char **args, t_shell *shell)
{
	char	*path;
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*target;

	path = args[1];
	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (!path)
	{
		target = get_env(shell->env, "HOME");
		if (!target)
		{
			print_error("cd: HOME not set", NULL);
			return (1);
		}
	}
	else if (ft_strcmp(path, "-") == 0)
	{
		target = get_env(shell->env, "OLDPWD");
		if (!target)
		{
			print_error("cd: OLDPWD not set", NULL);
			return (1);
		}
		write(STDOUT_FILENO, target, ft_strlen(target));
		write(STDOUT_FILENO, "\n", 1);
	}
	else
		target = path;
	if (chdir(target) != 0)
	{
		perror("cd");
		return (1);
	}
	if (getcwd(newpwd, sizeof(newpwd)) == NULL)
	{
		perror("cd: getcwd");
		return (1);
	}
	if (set_env(&(shell->env), "OLDPWD", oldpwd) != 0)
	{
		print_error("cd: Failed to set OLDPWD", NULL);
		return (1);
	}
	if (set_env(&(shell->env), "PWD", oldpwd) != 0)
	{
		print_error("cd: Failed to set PWD", NULL);
		return (1);
	}
	return (0);
}
