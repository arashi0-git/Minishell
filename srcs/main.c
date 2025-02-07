/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:05:10 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/07 14:34:28 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline("minishell$ ");
		add_history(input);
		printf("input: %s\n", input);
		free(input);
	}
	printf("exit\n");
	return (0);
}
