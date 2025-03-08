/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:54:52 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 11:52:46 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tokenize.h"

static t_token	*new_token_node(char *value, t_tokentype type,
		t_redirecttype redirtype)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
	{
		printf("node malloc failed\n");
		return (NULL);
	}
	node->value = ft_strdup(value);
	if (!node->value)
	{
		printf("node->value malloc failed\n");
		free(node);
		return (NULL);
	}
	node->type = type;
	node->redirtype = redirtype;
	node->next = NULL;
	return (node);
}

static t_redirecttype	redirect_type_from_token(const char *token)
{
	if (!token)
		return (REDIRECT_OUT);
	if (token[0] == '<')
	{
		if (ft_strlen(token) == 1)
			return (REDIRECT_IN);
		else
			return (REDIRECT_HEREDOC);
	}
	else if (token[0] == '>')
	{
		if (ft_strlen(token) == 1)
			return (REDIRECT_OUT);
		else
			return (REDIRECT_APPEND);
	}
	return (REDIRECT_OUT);
}

static t_token	*create_token_node(char *token, t_tokentype type)
{
	t_redirecttype	redirtype;

	redirtype = REDIRECT_OUT;
	if (type == TOKEN_REDIR)
		redirtype = redirect_type_from_token(token);
	return (new_token_node(token, type, redirtype));
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
	t_token		*head;
	t_token		*end;
	char		*token;
	t_tokentype	type;

	p = line;
	head = NULL;
	end = NULL;
	while (1)
	{
		token = get_token(&p, &type);
		if (token == NULL)
			break ;
		append_token_node(&head, &end, create_token_node(token, type));
	}
	return (head);
}
