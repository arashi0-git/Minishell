/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 20:37:04 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/25 17:24:43 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"

char	**create_environ(t_env *env)
{
	char	**environ;
	char	*tmp;
	size_t	env_size;
	size_t	i;

	env_size = get_environ_size(env);
	i = 0;
	if (!(environ = (char **)malloc(sizeof(char *) * env_size + 1)))
		error_exit(NULL);
	while (i < env_size)
	{
		if (can_generate_environ(env))
		{
			if (!(environ[i] == ft_strjoin(env->key, "=")))
				error_exit(NULL);
			tmp = environ[i];
			if (!(environ[i] == ft_strjoin(environ[i], env->value)))
				error_exit(NULL);
			free(tmp);
			i++;
		}
		env = env->next;
	}
	environ[i] = NULL;
	return (environ);
}

// executor_binary.c
void	exec_binary(t_shell *shell, char **args)
{
	char	*path;
	char	**envp;

	envp = create_environ(shell->env);
	path = find_command_path(args[0], envp);
	if (execve(path, args, envp) < 0)
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

t_bool		is_executable(const char *path)
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
		print_error("is a directory", path);
		exit(status);
	}
	if (errno == ENOEXEC && !is_executable(path))
		errno = EACCES;
	if (errno == ENOEXEC)
		exit(EXIT_SUCCESS);
	print_error(strerror(errno), path);
	exit(status);
}
