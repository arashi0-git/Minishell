/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 11:06:22 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/11 11:36:33 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	exec_echo(char **args)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		write(STDOUT_FILENO, args[i], ft_strlen(args[i]));
		if (args[i + 1])
			write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

void	test_echo(char **args, char *test_name, char *expected_output)
{
	int		original_stdout;
	int		pipe_fd[2];
	char	buffer[1024];
	int		result;
	int		bytes_read;

	// 出力をキャプチャするためのファイルディスクリプタ
	// 現在の標準出力を保存
	original_stdout = dup(STDOUT_FILENO);
	pipe(pipe_fd);
	dup2(pipe_fd[1], STDOUT_FILENO);
	// echoを実行
	result = exec_echo(args);
	// パイプをフラッシュしてclose
	fflush(stdout);
	close(pipe_fd[1]);
	// 出力を読み取る
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	buffer[bytes_read] = '\0';
	// 標準出力を元に戻す
	dup2(original_stdout, STDOUT_FILENO);
	// 結果を検証
	printf("return (value: %d\n", result);
	printf("Test: %s\n", test_name);
	printf("Expected: '%s'\n", expected_output);
	printf("Got     : '%s'\n", buffer);
	printf("Result  : %s\n\n", strcmp(buffer,
			expected_output) == 0 ? "PASS" : "FAIL");
}

int	main(int argc, char **argv)
{
	int	ret;

	(void)argc;
	if (!argv || !argv[0])
		return (1);
	ret = exec_echo(&argv[0]);
	return (ret);
}

/*
int	main(void) {
	// 基本的なテストケース
	printf("basic echo\n");
	char *test1[] = {"echo", "hello", NULL};
	test_echo(test1, "Basic echo", "hello\n");

	// -n オプションのテスト
	char *test2[] = {"echo", "-naaaaa", "hello", NULL};
	test_echo(test2, "Echo with -n", "hello");

	// 複数の引数
	printf("multiple arguments\n");
	char *test3[] = {"echo", "hello", "world", NULL};
	test_echo(test3, "Multiple arguments", "hello world\n");

	// 空の引数
	printf("Empty echo\n");
	char *test4[] = {"echo", NULL};
	test_echo(test4, "Empty echo", "\n");

	// スペースを含む引数
	printf("Arguments with spaces\n");
	char *test5[] = {"echo", "hello   world", NULL};
	test_echo(test5, "Argument with spaces", "hello   world\n");

	return (0);
}
*/
/*
 //man bash

  echo [-neE] [arg ...]
				Output  the  args,  separated  by spaces, followed by a newline.
				The return status is 0 unless a write error occurs.   If  -n  is
				specified, the trailing newline is suppressed.  If the -e option
				is given,  interpretation  of  the  following  backslash-escaped
				characters  is  enabled.  The -E option disables the interpreta‐
				tion of these escape characters, even on systems where they  are
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
