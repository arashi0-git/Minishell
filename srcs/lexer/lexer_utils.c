/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:10:31 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/08 12:54:00 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*process_quote(char **p)
{
	char	quote;
	char	*start;
	int		len;
	char	*token;

	start = *p;
	quote = **p;
	(*p)++;
	while (**p && **p != quote)
		(*p)++;
	if (**p == '\0')
	{
		printf("error quote not close\n");
		return (NULL);
	}
	if (**p == quote)
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		exit(1);
	// error_exit("quote token malloc failed\n");
	ft_strlcpy(token, start, len);
	token[len] = '\0';
	return (token);
}

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
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		exit(1);
	// error_exit("redir token malloc failed");
	ft_strlcpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_pipe(char **p)
{
	char	*token;

	token = malloc(2);
	if (!token)
		exit(1);
	// error_exit("pipe token malloc failed");
	return (NULL);
	token[0] = **p;
	token[1] = '\0';
	(*p)++;
	return (token);
}

static char	*process_word(char **p)
{
	char	*start;
	int		len;
	char	*token;

	start = *p;
	while (**p && !ft_isspace((unsigned char)**p) && **p != '\'' && **p != '"'
		&& **p != '>' && **p != '<' && **p != '|') // ft_
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		exit(1);
	// error_exit("word token malloc failed");
	ft_strlcpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char	*get_token(char **p, tokentype *token_type)
{
	while (**p && ft_isspace((unsigned char)**p))
		(*p)++;
	if (**p == '\0')
		return (NULL);
	if (**p == '\'' || **p == '"')
	{
		*token_type = TOKEN_COMMAND;
		return (process_quote(p));
	}
	else if (**p == '<' || **p == '>')
	{
		*token_type = TOKEN_REDIR;
		return (process_redir(p));
	}
	else if (**p == '|')
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
