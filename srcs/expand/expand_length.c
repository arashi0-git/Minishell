/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_length.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:37:19 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/28 16:17:17 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	process_single_quote(const char *str, size_t *i, size_t *len)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		(*len)++;
		(*i)++;
	}
	if (str[*i] != '\'')
		return (-1);
	(*i)++;
	return (0);
}

int	process_double_quote(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
		{
			if (process_dollar_length(str, shell, i, len) < 0)
				return (-1);
		}
		else
		{
			(*len)++;
			(*i)++;
		}
	}
	if (str[*i] != '"')
		return (-1);
	(*i)++;
	return (0);
}

int	process_dollar_length(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	if (str[*i + 1] == '?')
		return (process_dollar_question(shell, i, len));
	else
		return (process_dollar_others(str, i, len));
}

int	process_quote_expand(const char *str, t_shell *shell, size_t *i,
		size_t *len)
{
	int	length;

	length = 0;
	if (str[*i] == '\'')
		length = process_single_quote(str, i, len);
	else if (str[*i] == '"')
		length = process_double_quote(str, shell, i, len);
	return (length);
}
