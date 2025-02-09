/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:51:19 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/09 16:51:44 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	process_dollar_question(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	char	*status_str;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	*len += ft_strlen(status_str);
	free(status_str);
	*i += 2;
	return (0);
}

int	process_dollar_others(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	(*i)++;
	var_start = *i;
	while (str[*i] && (ft_isalnum((unsigned char)str[*i]) || str[*i] == '_'))
		(*i)++;
	var_len = *i - var_start;
	if (var_len == 0)
		(*len)++;
	else
	{
		var_name = ft_strdup(&str[var_start], var_len);
		if (!var_name)
			return (-1);
		value = getenv(var_name);
		free(var_name);
		if (value)
			*len += ft_strlen(value);
	}
	return (0);
}
