/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:37:04 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/15 11:27:12 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

char	**create_environ(t_env *env)
{
	char	**environ;
	size_t	env_size;
	size_t	i;

	env_size = get_environ_size(env);
	i = 0;
	environ = (char **)malloc(sizeof(char *) * (env_size + 1));
	if (!environ)
		error_exit(NULL);
	while (env && i < env_size)
	{
		if (can_generate_environ(env))
		{
			environ[i] = create_exec_env_string(env->key, env->value);
			i++;
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

	if (!args[0])
		exit(0);
	envp = create_environ(shell->env);
	path = find_command_path(args[0], envp);
	if (!path)
	{
		write(STDERR_FILENO, args[0], ft_strlen(args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
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
