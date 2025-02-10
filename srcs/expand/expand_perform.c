/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_perform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 16:54:02 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 13:13:43 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	expand_dollar(const char *str, t_shell *shell, t_expand *exp)
{
	if (str[exp->i + 1] == '?')
		return (expand_dollar_question(shell, exp));
	else
		return (expand_dollar_variable(str, exp));
}

static int	expand_single_quote(const char *str, t_expand *exp)
{
	exp->i++;
	while (str[exp->i] && str[exp->i] != '\'')
		exp->out[exp->out_index++] = str[exp->i++];
	if (str[exp->i] != '\'')
		return (-1);
	exp->i++;
	return (0);
}
static int	expand_double_quote(const char *str, t_shell *shell, t_expand *exp)
{
	exp->i++;
	while (str[exp->i] && str[exp->i] != '"')
	{
		if (str[exp->i] == '$')
		{
			if (expand_dollar(str, shell, exp) < 0)
				return (-1);
		}
		else
			exp->out[exp->out_index++] = str[exp->i++];
	}
	if (str[exp->i] != '"')
		return (-1);
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
		exp->out[exp->out_index++] = str[(exp->i)++];
	return (0);
}
