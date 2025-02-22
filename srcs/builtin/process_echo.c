/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:22 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/22 18:25:44 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_echo(char **args, int i, int opt_n)
{
	{
		while (args[i])
		{
			write(STDOUT_FILENO, args[i], strlen(args[i]));
			if (args[i + 1])
				write(STDOUT_FILENO, " ", 1);
			i++;
		}
		if (!opt_n)
			write(STDOUT_FILENO, "\n", 1);
	}
}

void	exec_echo(char **args)
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
	print_echo(args, i, opt_n);
}

// int	main(void)
// {
// 	char	*args[] = {"echo", "    -nnnnnnnnnnnn", "-nnnna", "hello", NULL};

// 	exec_echo(args);
// 	return (0);
// }
/*--------------------------------------test-------------------------------------------*/
// int	exec_echo(char **args)
// {
// 	int	i;
// 	int	newline;

// 	i = 1;
// 	newline = 1;
// 	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
// 	{
// 		newline = 0;
// 		i++;
// 	}
// 	while (args[i])
// 	{
// 		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
// 		if (args[i + 1])
// 			write(STDOUT_FILENO, " ", 1);
// 		i++;
// 	}
// 	if (newline)
// 		write(STDOUT_FILENO, "\n", 1);
// 	return (0);
// }

// void	test_echo(char **args, char *test_name, char *expected_output)
// {
// 	int		original_stdout;
// 	int		pipe_fd[2];
// 	char	buffer[1024];
// 	int		result;
// 	int		bytes_read;

// 	// 出力をキャプチャするためのファイルディスクリプタ
// 	// 現在の標準出力を保存
// 	original_stdout = dup(STDOUT_FILENO);
// 	pipe(pipe_fd);
// 	dup2(pipe_fd[1], STDOUT_FILENO);
// 	// echoを実行
// 	result = exec_echo(args);
// 	// パイプをフラッシュしてclose
// 	fflush(stdout);
// 	close(pipe_fd[1]);
// 	// 出力を読み取る
// 	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
// 	buffer[bytes_read] = '\0';
// 	// 標準出力を元に戻す
// 	dup2(original_stdout, STDOUT_FILENO);
// 	// 結果を検証
// 	printf("------**------\n");
// 	printf("return (value: %d\n", result);
// 	printf("Test: %s\n", test_name);
// 	printf("Expected: '%s'\n", expected_output);
// 	printf("Got     : '%s'\n", buffer);
// 	printf("Result  : %s\n\n", strcmp(buffer,
// 			expected_output) == 0 ? "PASS" : "FAIL");
// }

// int	main(void)
// {
// 	char	*test1[] = {"echo", "hello", NULL};
// 	char	*test2[] = {"echo", "-nnnnn", "hello", NULL};
// 	char	*test3[] = {"echo", "hello", "world", NULL};
// 	char	*test4[] = {"echo", NULL};
// 	char	*test5[] = {"echo", "hello   world", NULL};

// 	// 基本的なテストケース
// 	printf("basic echo\n");
// 	test_echo(test1, "Basic echo", "hello\n");
// 	// -n オプションのテスト
// 	test_echo(test2, "Echo with -n", "hello");
// 	// 複数の引数
// 	printf("multiple arguments\n");
// 	test_echo(test3, "Multiple arguments", "hello world\n");
// 	// 空の引数
// 	printf("Empty echo\n");
// 	test_echo(test4, "Empty echo", "\n");
// 	// スペースを含む引数
// 	printf("Arguments with spaces\n");
// 	test_echo(test5, "Argument with spaces", "hello   world\n");
// 	return (0);
// }

/*
	//man bash

	echo [-neE] [arg ...]
				Output  the  args,  separated  by spaces,
					followed by a newline.
				The return status is 0 unless a write error occurs.   If
					-n  is
				specified, the trailing newline is suppressed.  If the
					-e option
				is given,
						interpretation  of  the  following  backslash-escaped
				characters  is  enabled.  The
					-E option disables the interpreta‐
				tion of these escape characters,
					even on systems where they  are
				interpreted  by  default.  The xpg_echo shell option may be used
				to dynamically determine whether or not echo expands  these  es‐
				cape  characters by default.  echo does not interpret -- to mean
				the end of options.  echo interprets the  following  escape  se‐
				quences:
				\a     alert (bell)
				\b     backspace
				\c     suppress further output
				\e
				\E     an escape character
				\f     form feed
				\n     new line
				\r     carriage return
				\t     horizontal tab
				\v     vertical tab
				\\     backslash
				\0nnn  the  eight-bit  character  whose value is the octal value
						nnn (zero to three octal digits)
				\xHH   the eight-bit character whose value  is  the  hexadecimal
						value HH (one or two hex digits)
				\uHHHH the  Unicode (ISO/IEC 10646) character whose value is the
						hexadecimal value HHHH (one to four hex digits)
				\UHHHHHHHH
						the Unicode (ISO/IEC 10646) character whose value is  the
						hexadecimal value HHHHHHHH (one to eight hex digits)

*/
