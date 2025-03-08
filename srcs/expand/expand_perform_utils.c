/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_perform_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 13:15:55 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:28:40 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

int	expand_dollar_question(t_shell *shell, t_expand *exp)
{
	char	*status_str;
	size_t	t_len;

	status_str = ft_itoa(shell->exit_status);
	if (!status_str)
		return (-1);
	t_len = strlen(status_str);
	ft_memcpy(&exp->out[exp->out_index], status_str, t_len);
	exp->out_index += t_len;
	free(status_str);
	exp->i += 2;
	return (0);
}

// int	expand_dollar_question(t_shell *shell, t_expand *exp)
// {
// 	char	*status_str;
// 	size_t	t_len;

// 	status_str = ft_itoa(shell->exit_status);
// 	if (!status_str)
// 	{
// 		printf("ft_itoa returned NULL\n");
// 		return (-1);
// 	}
// 	t_len = strlen(status_str);
// 	ft_memcpy(&exp->out[exp->out_index], status_str, t_len);
// 	exp->out_index += t_len;
// 	free(status_str);
// 	exp->i += 2;
// 	return (0);
// }

static int	copy_variable(const char *value, t_expand *exp)
{
	size_t	v_len;

	if (!value)
		value = "";
	v_len = strlen(value);
	ft_memcpy(&exp->out[exp->out_index], value, v_len);
	exp->out_index += v_len;
	return (0);
}

int	expand_dollar_variable(const char *str, t_shell *shell, t_expand *exp)
{
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	exp->i++;
	var_start = exp->i;
	while (str[exp->i] && (isalnum((unsigned char)str[exp->i])
			|| str[exp->i] == '_'))
		exp->i++;
	var_len = exp->i - var_start;
	if (var_len == 0)
	{
		exp->out[exp->out_index++] = '$';
		return (0);
	}
	var_name = strndup(&str[var_start], var_len);
	if (!var_name)
		return (-1);
	value = get_env_value(shell->env, var_name);
	free(var_name);
	return (copy_variable(value, exp));
}
