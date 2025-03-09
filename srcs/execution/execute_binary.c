/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:37:04 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/03 15:07:08 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

char	**create_environ(t_env *env)
{
	char	**environ;
	char	*tmp;
	size_t	i;

	i = 0;
	environ = (char **)malloc(sizeof(char *) * (get_environ_size(env) + 1));
	if (!environ)
		error_exit(NULL);
	while (env && i < get_environ_size(env))
	{
		if (can_generate_environ(env))
		{
			environ[i] = ft_strjoin(env->key, "=");
			if (!environ[i])
				error_exit(NULL);
			tmp = environ[i];
			environ[i] = ft_strjoin(tmp, env->value);
			free(tmp);
			if (!environ[i++])
				error_exit(NULL);
		}
		env = env->next;
	}
	environ[i] = NULL;
	return (environ);
}

void	exec_binary(t_shell *shell, char **args)
{
	char	*path;
	char	**envp;

	if (!args[0] || !args)
		return ;
	envp = create_environ(shell->env);
	path = find_command_path(args[0], envp);
	if (!path)
	{
		printf("%s: command not found\n", args[0]);
		shell->exit_status = 127;
		exit(shell->exit_status);
	}
	execve(path, args, envp);
	handle_execve_error(path);
	free(path);
	free_array(envp);
}

t_bool	is_directory(const char *path)
{
	t_stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (FALSE);
	if (S_ISDIR(path_stat.st_mode))
		return (TRUE);
	return (FALSE);
}

t_bool	is_executable(const char *path)
{
	t_stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (FALSE);
	if ((path_stat.st_mode & S_IXUSR) != S_IXUSR)
		return (FALSE);
	if ((path_stat.st_mode & S_IRUSR) != S_IRUSR)
		return (FALSE);
	return (TRUE);
}

void	handle_execve_error(char *path)
{
	int	status;

	if (errno == ENOENT)
		status = STATUS_CMD_NOT_FOUND;
	else
		status = STATUS_CMD_NOT_EXECUTABLE;
	if (is_directory(path))
	{
		print_error("is a directory", NULL, path);
		exit(status);
	}
	if (errno == ENOEXEC && !is_executable(path))
		errno = EACCES;
	if (errno == ENOEXEC)
		exit(EXIT_SUCCESS);
	print_error(strerror(errno), NULL, path);
	exit(status);
}
