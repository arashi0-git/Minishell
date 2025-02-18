/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export_utils2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:51:11 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/09 17:53:48 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env		*set_env_helper(const char *key, const char *value);
void		free_env(t_env *env);

static char	*create_env_string(const char *key, const char *value)
{
	char	*result;
	size_t	len;

	if (!value)
		return (ft_strdup(key));
	len = ft_strlen(key) + ft_strlen(value) + 2;
	result = malloc(len);
	if (!result)
		return (NULL);
	ft_strlcpy(result, key, len);
	ft_strlcat(result, "=", len);
	ft_strlcat(result, value, len);
	return (result);
}

static int	get_env_size(t_env *env)
{
	int		size;
	t_env	*current;

	size = 0;
	current = env;
	while (current)
	{
		size++;
		current = current->next;
	}
	return (size);
}

static void	selection_sort_part(char **arr, int size, int start)
{
	int		j;
	int		min_idx;
	char	*temp;

	min_idx = start;
	j = start + 1;
	while (j < size)
	{
		if (ft_strcmp(arr[j], arr[min_idx]) < 0)
			min_idx = j;
		j++;
	}
	if (min_idx != start)
	{
		temp = arr[start];
		arr[start] = arr[min_idx];
		arr[min_idx] = temp;
	}
}

static void	selection_sort(char **arr, int size)
{
	int	i;

	i = 0;
	while (i < size - 1)
	{
		selection_sort_part(arr, size, i);
		i++;
	}
}

static char	**env_to_array(t_env *env, int size)
{
	char	**arr;
	t_env	*current;
	int		i;

	arr = malloc(sizeof(char *) * (size + 1));
	if (!arr)
		return (NULL);
	current = env;
	i = 0;
	while (current && i < size)
	{
		if (!(arr[i] = create_env_string(current->key, current->value)))
		{
			while (--i >= 0)
				free(arr[i]);
			free(arr);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	arr[size] = NULL;
	return (arr);
}

char	**sort_env(t_env *env)
{
	char	**env_arr;
	int		size;

	size = get_env_size(env);
	env_arr = env_to_array(env, size);
	if (!env_arr)
		return (NULL);
	selection_sort(env_arr, size);
	return (env_arr);
}
