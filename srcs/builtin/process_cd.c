/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:14 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/03 15:06:31 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

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
			write(STDOUT_FILENO, oldpwd, ft_strlen(oldpwd));
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

	if (getcwd(oldpwd, sizeof(oldpwd)) == NULL)
	{
		print_error("cd: getcwd", NULL, NULL);
		return (1);
	}
	target = resolve_path(args[1], shell);
	if (!target)
	{
		print_error("cd: path not set", NULL, NULL);
		return (1);
	}
	if (chdir(target) != 0)
	{
		print_error(strerror(errno), "cd", target);
		return (1);
	}
	if (update_pwd_env(shell, oldpwd) != 0)
	{
		print_error("cd: Failed to update PWD", NULL, NULL);
		return (1);
	}
	return (0);
}

/*
#include <errno.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//初期化関数
int	initialize_shell_env(t_shell *shell)
{
	char			current_dir[PATH_MAX];
	char			*home_dir;
	struct passwd	*pw;

	if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
		perror("getcwd");
		return (1);
	}
	if (set_env(&shell->env, "PWD", current_dir) != 0 ||
		set_env(&shell->env, "OLDPWD", current_dir) != 0) {
		return (1);
	}
	home_dir = getenv("HOME");
	if (!home_dir) {
		pw = getpwuid(getuid());
		if (!pw)
			return (1);
		home_dir = pw->pw_dir;
	}
	if (set_env(&shell->env, "HOME", home_dir) != 0)
		return (1);
	return (0);
}

// メイン関数
int	main(int argc, char *argv[])
{
	char	before[PATH_MAX];
	char	after[PATH_MAX];
	t_shell	shell;
	char	*args[3];
	int		return_value;

	getcwd(before, sizeof(before));
	printf("Before: %s\n", before);
	shell = {0};
	if (initialize_shell_env(&shell) != 0)
		return (1);
	args[0] = "cd";
	args[1] = (argc > 1) ? argv[1] : NULL;
	args[2] = NULL;
	return_value = exec_cd(args, &shell);
	getcwd(after, sizeof(after));
	printf("\nAfter: %s\n", after);
	printf("\nexec_cd:return_value:%d\n", return_value);
}
*/
