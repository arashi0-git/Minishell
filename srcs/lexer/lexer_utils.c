/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 16:10:31 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/03 15:13:25 by aryamamo         ###   ########.fr       */
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
	if (**p == quote)
		(*p)++;
	len = *p - start;
	token = malloc(len + 1);
	if (!token)
		exit(1);
	// error_exit("quote token malloc failed\n");
	ft_strlcpy(token, start, len); // ft_
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
	ft_strlcpy(token, start, len); // ft_
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
	ft_strlcpy(token, start, len); // ft_
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

static t_token	*new_token_node(char *value, tokentype type,
		RedirectType redirType)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		exit(1);
	// error_exit("node malloc failed");
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
		if (ft_strlen(token) == 1)
			return (REDIR_IN);
		else
			return (REDIR_HEREDOC);
	}
	else if (token[0] == '>')
	{
		if (ft_strlen(token) == 1)
			return (REDIR_OUT);
		else
			return (REDIR_APPEND);
	}
	return (REDIR_OUT);
}

static t_token	*create_token_node(char *token, tokentype type)
{
	RedirectType	redirType;

	redirType = REDIR_OUT;
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
// void	print_token_list(t_token *list)
// {
// 	int	index;

// 	index = 0;
// 	while (list)
// 	{
// 		printf("Token[%d]: [%s] ", index, list->value);
// 		if (list->type == TOKEN_COMMAND)
// 			printf("Type: COMMAND");
// 		else if (list->type == TOKEN_PIPE)
// 			printf("Type: PIPE");
// 		else if (list->type == TOKEN_REDIR)
// 		{
// 			printf("Type: REDIR, ");
// 			if (list->redirType == REDIRECT_IN)
// 				printf("REDIRECT_IN");
// 			else if (list->redirType == REDIRECT_OUT)
// 				printf("REDIRECT_OUT");
// 			else if (list->redirType == REDIRECT_APPEND)
// 				printf("REDIRECT_APPEND");
// 			else if (list->redirType == REDIRECT_HEREDOC)
// 				printf("REDIRECT_HEREDOC");
// 		}
// 		printf("\n");
// 		index++;
// 		list = list->next;
// 	}
// }

// /* リストのメモリ解放 */
// void	free_token_list(t_token *list)
// {
// 	t_token	*tmp;

// 	while (list)
// 	{
// 		tmp = list;
// 		list = list->next;
// 		free(tmp->value);
// 		free(tmp);
// 	}
// }

/* --- テスト用 main --- */
#ifdef TEST_LEXER_LIST_ATTR

int	main(void)
{
	char	input[] = "cat < file1 | grep \"a b\" |wc -l << file2";
	t_token	*token_list;

	token_list = tokenize_list(input);
	print_token_list(token_list);
	free_token_list(token_list);
	return (0);
}

#endif