/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 20:53:48 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/07 04:26:36 by retoriya         ###   ########.fr       */
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

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dest_ptr;
	unsigned char	*src_ptr;
	size_t			i;

	if (!dest && !src)
		return (NULL);
	dest_ptr = (unsigned char *)dest;
	src_ptr = (unsigned char *)src;
	i = 0;
	while (i < n)
	{
		dest_ptr[i] = src_ptr[i];
		i++;
	}
	return (dest);
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
