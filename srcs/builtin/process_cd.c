/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:14 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/14 14:07:20 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
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
	if (set_env(&(shell->env), "PWD", newpwd) != 0)
	{
		print_error("cd: Failed to set PWD", NULL);
		return (1);
	}
	return (0);
}
*/

static char	*resolve_path(char *path, t_shell *shell)
{
	char	*oldpwd;

	oldpwd = NULL;
	if (!path)
		return (get_env(shell->env, "HOME"));
	if (ft_strcmp(path, "-") == 0)
	{
		oldpwd = get_env(shell->env, "OLDPWD");
		if (oldpwd)
			write(STDIN_FILENO, oldpwd, ft_strlen(oldpwd));
		return (oldpwd);
	}
	return (path);
}

static int	update_pwd_env(t_shell *shell, char *oldpwd)
{
	char	newpwd[PATH_MAX];

	if (getcwd(newpwd, sizeof(newpwd)) == NULL)
		return (1);
	if (set_env(&(shell->env), "OLDPWD", oldpwd) != 0 || set_env(&(shell->env),
			"PWD", newpwd) != 0)
		return (1);
	return (0);
}

int	exec_cd(char **args, t_shell *shell)
{
	char	oldpwd[PATH_MAX];
	char	*target;

	if (getcwd(oldcwd, sizeof(oldpwd)) == NULL)
	{
		print_error("cd: getcwd", NULL);
		return (1);
	}
	target = resolve_path(args[1], shell);
	if (!target)
	{
		print_error("cd: path not set", NULL);
		return (1);
	}
	if (chdir(target) != 0)
	{
		print_error("cd", NULL);
		return (1);
	}
	if (update_pwd_env(shell, oldpwd) != 0)
    {
        print_error("cd: Failed to update PWD", NULL);
        return (1);
    }
    return (0);
}

