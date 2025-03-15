/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_commond_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:58:24 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/15 11:29:28 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*create_path(const char *dir, const char *cmd)
{
	char	*path;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!path)
		return (NULL);
	while (dir[i])
	{
		path[i] = dir[i];
		i++;
	}
	path[i++] = '/';
	while (cmd[j])
		path[i++] = cmd[j++];
	path[i] = '\0';
	return (path);
}

static char	*get_env_path(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

static char	*check_dir_path(char *dir_start, const char *cmd)
{
	char	*temp_path;
	int		j;
	char	orig;

	j = 0;
	while (dir_start[j] && dir_start[j] != ':')
		j++;
	orig = dir_start[j];
	if (orig == ':')
		dir_start[j] = '\0';
	temp_path = create_path(dir_start, cmd);
	if (orig == ':')
		dir_start[j] = orig;
	if (temp_path && access(temp_path, X_OK) == 0)
		return (temp_path);
	free(temp_path);
	return (NULL);
}

char	*find_command_path(const char *cmd, char **envp)
{
	char	*path;
	char	*dir_start;
	char	*temp_path;
	int		j;

	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	path = get_env_path(envp);
	if (!path)
		return (NULL);
	dir_start = path;
	while (*dir_start)
	{
		temp_path = check_dir_path(dir_start, cmd);
		if (temp_path)
			return (temp_path);
		j = 0;
		while (dir_start[j] && dir_start[j] != ':')
			j++;
		dir_start = dir_start + j;
		if (*dir_start)
			dir_start++;
	}
	return (NULL);
}
