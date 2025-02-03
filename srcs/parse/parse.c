/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 14:27:11 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/02 14:29:11 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*peek_token(t_parser *token)
{
	if (token->pos < token->token_count)
		return (token->tokens[token->pos]);
	return (NULL);
}