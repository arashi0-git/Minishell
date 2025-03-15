/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_split.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 23:18:26 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/15 11:35:25 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand.h"

static int	count_words(const char *s)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (isspace((unsigned char)*s))
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*alloc_word(const char *start, size_t len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, start, len);
	word[len] = '\0';
	return (word);
}

static char	*get_next_word(const char **s_ptr)
{
	const char	*s = *s_ptr;
	const char	*word_start;
	size_t		len;
	char		*word;

	while (*s && ft_isspace((unsigned char)*s))
		s++;
	if (!*s)
		return (NULL);
	len = 0;
	word_start = s;
	while (s[len] && !ft_isspace((unsigned char)s[len]))
		len++;
	word = alloc_word(word_start, len);
	if (!word)
		return (NULL);
	*s_ptr = s + len;
	return (word);
}

void	free_words(char **words)
{
	int	i;

	i = 0;
	if (!words)
		return ;
	while (words[i])
	{
		free(words[i]);
		i++;
	}
	free(words);
}

char	**split_words(const char *s)
{
	int		word_count;
	char	**result;
	int		index;
	char	*word;

	word_count = count_words(s);
	result = malloc((word_count + 1) * sizeof(char *));
	index = 0;
	if (!result)
		return (NULL);
	while (1)
	{
		word = get_next_word(&s);
		if (!word)
			break ;
		result[index++] = word;
	}
	result[index] = NULL;
	return (result);
}
