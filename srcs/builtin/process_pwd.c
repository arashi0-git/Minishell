/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 16:16:43 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/15 11:23:39 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin.h"

int	exec_pwd(t_shell *shell)
{
	char	*str;

	str = NULL;
	str = getcwd(NULL, 0);
	if (str == NULL)
		return (1);
	ft_putendl_fd(str, STDOUT_FILENO);
	free(str);
	shell->exit_status = 0;
	return (0);
}
/*
 //cc -Wall -Wextra -Werror process_pwd.c -I../../include -L../../libft -lft
	-o process_pwd
int	main(void)
{
	process_pwd();
	return (0);
}
*/
/*
requirements:pwd with no options

   man bash

 pwd [-LP]
				Print  the  absolute  pathname of the current working directory.
				The pathname printed contains no symbolic links if the -P option
				is supplied or the -o physical option to the set builtin command
				is enabled.  If the -L option is used, the pathname printed  may
				contain  symbolic links.  The return status is 0 unless an error
				occurs while reading the name of the current directory or an in‐
				valid option is supplied.


DESCRIPTION-getcwd
		These  functions return a null-terminated string containing an absolute
		pathname that is the current working directory of the calling  process.
		The  pathname  is  returned as the function result and via the argument
		buf, if present.

		The getcwd() function copies an absolute pathname of the current  work‐
		ing directory to the array pointed to by buf, which is of length size.

		If  the  length  of the absolute pathname of the current working direc‐
		tory, including the terminating null byte, exceeds size bytes, NULL  is
		returned,  and  errno is set to ERANGE; an application should check for
		this error, and allocate a larger buffer if necessary.

		As an extension to the POSIX.1-2001 standard,  glibc's  getcwd()  allo‐
		cates  the  buffer dynamically using malloc(3) if buf is NULL.  In this
		case, the allocated buffer has the length size  unless  size  is  zero,
		when  buf  is allocated as big as necessary.  The caller should free(3)
		the returned buffer.

*/
