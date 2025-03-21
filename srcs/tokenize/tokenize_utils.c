/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:10:31 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/23 01:10:07 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

static char	*process_redir(char **p)
{
	char	*start;
	char	c;
	int		len;
	char	*token;

	start = *p;
	c = **p;
	(*p)++;
	if (**p == c)
		(*p)++;
	len = *p - start + 1;
	token = malloc(len + 1);
	if (!token)
	{
		printf("redirect malloc failed\n");
		return (NULL);
	}
	ft_strlcpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_pipe(char **p)
{
	char	*token;

	token = malloc(2);
	if (!token)
	{
		printf("token malloc failed\n");
		return (NULL);
	}
	token[0] = **p;
	token[1] = '\0';
	(*p)++;
	return (token);
}

static void	skip_quote(char **p)
{
	char	quote;

	quote = **p;
	(*p)++;
	while (**p && **p != quote)
		(*p)++;
	if (**p == quote)
		(*p)++;
}

static char	*process_word(char **p)
{
	char	*start;
	int		len;
	char	*token;

	start = *p;
	while (**p && !ft_isspace((unsigned char)**p) && **p != '>' && **p != '<'
		&& **p != '|')
	{
		if (**p == '\'' || **p == '"')
			skip_quote(p);
		else
			(*p)++;
	}
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
	{
		printf("token malloc failed\n");
		return (NULL);
	}
	ft_strlcpy(token, start, len + 1);
	return (token);
}

char	*get_token(char **p, t_tokentype *token_type)
{
	while (**p && (**p == ' ' || **p == '\t'))
		(*p)++;
	if (**p == '\0' || ft_isascii(**p) == 0)
		return (NULL);
	if (**p == '<' || **p == '>')
	{
		*token_type = TOKEN_REDIR;
		return (process_redir(p));
	}
	else if (**p && **p == '|')
	{
		*token_type = TOKEN_PIPE;
		return (process_pipe(p));
	}
	else
	{
		*token_type = TOKEN_COMMAND;
		return (process_word(p));
	}
}
