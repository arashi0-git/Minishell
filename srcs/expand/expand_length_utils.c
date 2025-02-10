/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_length_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:20:54 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 13:29:50 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_dollar_question(t_shell *shell, size_t *i, size_t *len)
{
	char	*status_str;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	*len += strlen(status_str);
	free(status_str);
	*i += 2;
	return (0);
}

int	process_dollar_others(const char *str, size_t *i, size_t *len)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	(*i)++;
	var_start = *i;
	while (str[*i] && (isalnum((unsigned char)str[*i]) || str[*i] == '_'))
		(*i)++;
	var_len = *i - var_start;
	if (var_len == 0)
		(*len)++;
	else
	{
		var_name = strndup(&str[var_start], var_len);
		if (!var_name)
			return (-1);
		value = getenv(var_name);
		free(var_name);
		if (value)
			*len += strlen(value);
	}
	return (0);
}
