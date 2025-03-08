/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_identifier.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 15:40:20 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 15:40:22 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_valid_identifier(const char *str)
{
	char	*equal_pos;
	size_t	key_length;
	size_t	i;

	equal_pos = NULL;
	i = 0;
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	equal_pos = ft_strchr(str, '=');
	if (equal_pos)
		key_length = equal_pos - str;
	else
		key_length = ft_strlen(str);
	i = 1;
	while (i < key_length)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
