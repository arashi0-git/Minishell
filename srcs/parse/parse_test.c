/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 15:40:48 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/04 15:44:14 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* --- トークン関連 --- */

typedef enum token_type
{
	TOKEN_COMMAND,
	TOKEN_PIPE,
	TOKEN_REDIR
}					tokentype;

typedef enum
{
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	REDIRECT_HEREDOC
}					RedirectType;

typedef struct s_token
{
	char			*value;
	tokentype		type;
	RedirectType	redirType;
	struct s_token	*next;
}					t_token;

/* --- 単語（トークン）の処理 --- */

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
		printf("error: quote not closed\n");
		return (NULL);
	}
	if (**p == quote)
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
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
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

static char	*process_pipe(char **p)
{
	char	*token;

	token = malloc(2);
	if (!token)
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
	while (**p && !isspace((unsigned char)**p) && **p != '\'' && **p != '"'
		&& **p != '>' && **p != '<' && **p != '|')
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		return (NULL);
	strncpy(token, start, len);
	token[len] = '\0';
	return (token);
}

char	*get_token(char **p, tokentype *token_type)
{
	while (**p && isspace((unsigned char)**p))
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

static t_token	*new_token_node(char *value, tokentype type,
		RedirectType redirType)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->redirType = redirType;
	node->next = NULL;
	return (node);
}

static RedirectType	redirect_type_from_token(const char *token)
{
	if (token[0] == '<')
	{
		if (strlen(token) == 1)
			return (REDIRECT_IN);
		else
			return (REDIRECT_HEREDOC);
	}
	else if (token[0] == '>')
	{
		if (strlen(token) == 1)
			return (REDIRECT_OUT);
		else
			return (REDIRECT_APPEND);
	}
	return (REDIRECT_OUT);
}

static t_token	*create_token_node(char *token, tokentype type)
{
	RedirectType	redirType;

	redirType = REDIRECT_OUT;
	if (type == TOKEN_REDIR)
		redirType = redirect_type_from_token(token);
	return (new_token_node(token, type, redirType));
}

static void	append_token_node(t_token **head, t_token **end, t_token *node)
{
	if (!*head)
	{
		*head = node;
		*end = node;
	}
	else
	{
		(*end)->next = node;
		*end = node;
	}
}

t_token	*tokenize_list(char *line)
{
	char		*p;
	char		*token;
	tokentype	type;
	t_token		*head;
	t_token		*end;
	t_token		*node;

	p = line;
	head = NULL;
	end = NULL;
	while ((token = get_token(&p, &type)) != NULL)
	{
		node = create_token_node(token, type);
		append_token_node(&head, &end, node);
	}
	return (head);
}

void	print_token_list(t_token *list)
{
	int	index;

	index = 0;
	while (list)
	{
		printf("Token[%d]: [%s] ", index, list->value);
		if (list->type == TOKEN_COMMAND)
			printf("Type: COMMAND");
		else if (list->type == TOKEN_PIPE)
			printf("Type: PIPE");
		else if (list->type == TOKEN_REDIR)
		{
			printf("Type: REDIR, ");
			if (list->redirType == REDIRECT_IN)
				printf("REDIRECT_IN");
			else if (list->redirType == REDIRECT_OUT)
				printf("REDIRECT_OUT");
			else if (list->redirType == REDIRECT_APPEND)
				printf("REDIRECT_APPEND");
			else if (list->redirType == REDIRECT_HEREDOC)
				printf("REDIRECT_HEREDOC");
		}
		printf("\n");
		index++;
		list = list->next;
	}
}

void	free_token_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}

/*---parse---*/

typedef struct s_cmd
{
	char			*command;
	char			**args;
	int				argc;
	char			*infile;
	char			*outfile;
	int				append;
	struct s_cmd	*next;
}					t_cmd;