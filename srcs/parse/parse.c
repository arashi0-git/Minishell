/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:27:11 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/04 10:45:24 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = ft_calloc(1, sizeof(*node));
	if (!node)
	{
		printf("error calloc of parse node\n");
		return (NULL);
	}
	node->kind = kind;
	return (node);
}

t_token	*tokendup(t_token *token)
{
	char	*word;

	word = ft_strdup(token->word);
	if (word == NULL)
	{
		printf("strdup error of token");
		return (NULL);
	}
	return (new_token(word, token->type));
}

void	append_token(t_token **tokens, t_token *token)
{
	if (*tokens == NULL)
	{
		*tokens = token;
		return ;
	}
	append_token(&(*tokens)->next, token);
}

t_node	*parse(t_token *token)
{
	t_node	*node;

	node = new_node(ND_SIMPLE_CMD);
	while (token)
	{
		if (token->type == TOKEN_COMMAND)
			append_token(&node->args, tokendup(token));
		else
			todo("Implement parser");
		token = token->next;
	}
	return (node);
}
