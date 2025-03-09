/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_next_line_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:53:48 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/07 04:39:55 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	init_string(t_string *line)
{
	line->str = (char *)malloc(BUFFER_SIZE);
	if (line->str)
	{
		line->str[0] = '\0';
		line->len = 0;
		line->capa = BUFFER_SIZE;
	}
	else
	{
		line->len = 0;
		line->capa = 0;
	}
}

int	finalize_line(t_string *line)
{
	if (line->len == 0)
	{
		free(line->str);
		return (1);
	}
	return (0);
}

// int	ft_putc(t_string *line, char c)
// {
// 	char	*str_tmp;
// 	size_t	i;

// 	i = line->len;
// 	if (line->len + 1 >= line->capa)
// 	{
// 		str_tmp = line->str;
// 		line->capa += BUFFER_SIZE;
// 		line->str = (char *)malloc(line->capa);
// 		if (!line->str)
// 		{
// 			free(str_tmp);
// 			return (PUTC_ERROR);
// 		}
// 		i = 0;
// 		while (str_tmp && i < line->len)
// 		{
// 			line->str[i] = str_tmp[i];
// 			i++;
// 		}
// 		free(str_tmp);
// 	}
// 	line->str[line->len++] = c;
// 	line->str[line->len] = '\0';
// 	return (0);
// }
