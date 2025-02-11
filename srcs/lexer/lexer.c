/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:54:52 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/10 13:53:41 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*new_token_node(char *value, tokentype type,
		RedirectType redirType)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
	{
		printf("node malloc failed\n");
		return (NULL);
	}
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
