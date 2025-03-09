/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_perform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:54:02 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/09 20:24:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int	expand_dollar(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i + 1] == '?')
		return (expand_dollar_question(shell, exp));
	else
		return (expand_dollar_variable(str, shell, exp));
}

static int	expand_single_quote(const char *str, t_expand *exp)
{
	if (str[exp->i] != '\'')
		return (-1);
	exp->i++;
	while (str[exp->i] && str[exp->i] != '\'')
	{
		if (exp->out_index >= exp->max)
			return (-1);
		exp->out[exp->out_index++] = str[exp->i];
		exp->i++;
	}
	if (str[exp->i] != '\'')
	{
		printf("Error: single quote is not closed\n");
		return (-1);
	}
	exp->i++;
	return (0);
}

static int	expand_double_quote(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i] != '"')
		return (-1);
	exp->i++;
	while (str[exp->i] && str[exp->i] != '"')
	{
		if (str[exp->i] == '$')
		{
			if (expand_dollar(str, shell, exp) < 0)
				return (-1);
		}
		else
		{
			if (exp->out_index >= exp->max)
				return (-1);
			exp->out[exp->out_index++] = str[exp->i];
			exp->i++;
		}
	}
	if (str[exp->i] != '"')
	{
		printf("Error: double quote is not closed\n");
		return (-1);
	}
	exp->i++;
	return (0);
}

int	process_expansion_char(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i] == '\'')
	{
		if (expand_single_quote(str, exp) < 0)
			return (-1);
	}
	else if (str[exp->i] == '"')
	{
		if (expand_double_quote(str, shell, exp) < 0)
			return (-1);
	}
	else if (str[exp->i] == '$')
	{
		if (expand_dollar(str, shell, exp) < 0)
			return (-1);
	}
	else
	{
		if (exp->out_index >= exp->max)
			return (-1);
		exp->out[exp->out_index++] = str[exp->i];
		exp->i++;
	}
	return (0);
}

char	*perform_expansion(const char *str, t_shell *shell, size_t total_len)
{
	t_expand	*exp;
	char		*result;

	exp = init_expand(total_len);
	if (!exp || !str)
		return (NULL);
	while (str[exp->i] != '\0')
	{
		if (process_expansion_char(str, shell, exp) < 0)
		{
			free(exp->out);
			free(exp);
			return (NULL);
		}
	}
	if (exp->out_index < exp->max + 1)
		exp->out[exp->out_index] = '\0';
	else
		exp->out[exp->max] = '\0';
	result = exp->out;
	free(exp);
	return (result);
}
