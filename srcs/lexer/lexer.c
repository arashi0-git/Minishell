/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:54:52 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/03 13:41:45 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_tokens(char *str)
{
	int		count;
	char	*p;
	char	*token;

	count = 0;
	p = str;
	while (1)
	{
		token = get_token(&p);
		if (!token)
			break ;
		count++;
		free(token);
	}
	return (count);
}

char	**token_split(char *str)
{
	int		n_tokens;
	char	**tokens;
	int		i;
	char	*p;
	char	*token;

	n_tokens = count_tokens(str);
	tokens = malloc(sizeof(char *) * (n_tokens + 1));
	if (!tokens)
		exit(1);
	// error_exit("tokens malloc failed");
	// bashでmallocが失敗するとメッセージ表示して続行したい
	i = 0;
	p = str;
	while ((token = get_token(&p)) != NULL)
		tokens[i++] = token;
	tokens[i] = NULL;
	return (tokens);
}
