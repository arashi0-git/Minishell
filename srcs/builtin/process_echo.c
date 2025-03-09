/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:22 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 23:37:10 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"

int	print_echo(char **args, int i, int opt_n)
{
	{
		while (args[i])
		{
			write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
			if (args[i + 1])
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
		if (!opt_n)
			write(STDOUT_FILENO, "\n", 1);
	}
	return (0);
}

int	exec_echo(char **args)
{
	int	i;
	int	j;
	int	opt_n;

	opt_n = 0;
	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-')
		{
			j = 1;
			while (args[i][j] == 'n')
				j++;
			if (args[i][j] == '\0')
			{
				opt_n = 1;
				i++;
				continue ;
			}
		}
		break ;
	}
	return (print_echo(args, i, opt_n));
}
