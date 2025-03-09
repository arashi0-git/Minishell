/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:19:33 by aryamamo          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2025/02/01 11:29:15 by aryamamo         ###   ########.fr       */
=======
/*   Updated: 2025/03/09 16:03:35 by aryamamo         ###   ########.fr       */
>>>>>>> 6ce31a0 (add)
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

<<<<<<< HEAD
char	*ft_strndup(char *str, size_t n)
=======
char	*ft_strndup(const char *str, size_t n)
>>>>>>> 6ce31a0 (add)
{
	size_t	i;
	char	*copy;

	i = 0;
	if (str == NULL)
		return (NULL);
	copy = (char *)malloc((n + 1) * sizeof(char));
	if (copy == NULL)
		return (NULL);
	while (i < n)
	{
		copy[i] = str[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}
