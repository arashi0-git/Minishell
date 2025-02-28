/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:07:22 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/28 16:24:19 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include <errno.h>
# include <expand.h>
# include <libft.h>
# include <limits.h>
# include <parse.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <tokenize.h>
# include <unistd.h>

/*---builtin func---*/
int	is_builtin(char **args);
int	exec_builtin(char **args, t_shell *shell);

#endif