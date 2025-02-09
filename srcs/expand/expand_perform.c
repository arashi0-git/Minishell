/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_perform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:54:02 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/09 17:19:28 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	expand_dollar_question(const char *str, t_shell *shell, char *out,
		size_t *i, size_t *out_index)
{
	char	*status_str;
	size_t	t_len;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	t_len = strlen(status_str);
	memcpy(&out[*out_index], status_str, t_len);
	*out_index += t_len;
	free(status_str);
	*i += 2;
	return (0);
}

static int	expand_dollar_variable(const char *str, t_shell *shell, char *out,
		size_t *i, size_t *out_index)
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
		out[(*out_index)++] = '$';
	else
	{
		var_name = strndup(&str[var_start], var_len);
		if (!var_name)
			return (-1);
		value = getenv(var_name);
		free(var_name);
		if (!value)
			value = "";
		*out_index += memcpy(&out[*out_index], value, strlen(value)) - out;
	}
	return (0);
}

static int	expand_dollar(const char *str, t_shell *shell, char *out, size_t *i,
		size_t *out_index)
{
	if (str[*i + 1] == '?')
		return (expand_dollar_question(str, shell, out, i, out_index));
	else
		return (expand_dollar_variable(str, shell, out, i, out_index));
}

int	process_expansion_char(const char *str, t_shell *shell, char *out,
		size_t *i, size_t *out_index)
{
	if (str[*i] == '\'')
	{
		if (expand_single_quote(str, out, i, out_index) < 0)
			return (-1);
	}
	else if (str[*i] == '"')
	{
		if (expand_double_quote(str, shell, out, i, out_index) < 0)
			return (-1);
	}
	else if (str[*i] == '$')
	{
		if (expand_dollar(str, shell, out, i, out_index) < 0)
			return (-1);
	}
	else
		out[(*out_index)++] = str[(*i)++];
	return (0);
}
