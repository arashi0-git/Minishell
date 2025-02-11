/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:03:31 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/11 13:11:34 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//たたき台
int    cmd_unset(char **args, t_shell *shell)
{
    int        i;
    int        status;

    if (!args[1])
        return (0);  // 引数がない場合は成功として扱う

    i = 1;
    status = 0;
    while (args[i])
    {
        if (!is_valid_identifier(args[i]))
        {
            error_invalid_identifier("unset", args[i]);
            status = 1;
        }
        else
            remove_env_var(shell, args[i]);
        i++;
    }
    return (status);
}

