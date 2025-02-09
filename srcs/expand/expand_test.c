/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 10:02:32 by aryamamo          #+#    #+#             */
/*   Updated: 2025/02/09 14:48:47 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/minishell.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct s_shell
{
	int			exit_status;

}				t_shell;

static size_t	get_digit_count(int n)
{
	size_t	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	size_t			len;
	char			*result;
	unsigned int	num;

	len = get_digit_count(n);
	result = (char *)malloc(len + 1);
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		num = -n;
	}
	else
		num = n;
	if (num == 0)
		result[0] = '0';
	result[len] = '\0';
	while (num != 0)
	{
		result[len - 1] = (num % 10) + '0';
		num = num / 10;
		len--;
	}
	return (result);
}

size_t	expand_length(const char *str, t_shell *shell)
{
	size_t	len;
	size_t	i;
	char	*status_str;
	size_t	var_start;
	size_t	var_len;
	char	*var_name;
	char	*value;

	len = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
			{
				len++;
				i++;
			}
			if (str[i] != '\'')
				return ((size_t)-1);
			i++;
		}
		else if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				{
					if (str[i + 1] == '?')
					{
						status_str = ft_itoa(shell->exit_status);
						if (!status_str)
							return ((size_t)-1);
						len += strlen(status_str);
						free(status_str);
						i += 2;
					}
					else
					{
						i++;
						var_start = i;
						while (str[i] && (isalnum((unsigned char)str[i])
								|| str[i] == '_'))
							i++;
						var_len = i - var_start;
						if (var_len == 0)
							len++;
						else
						{
							var_name = strndup(&str[var_start], var_len);
							if (!var_name)
								return ((size_t)-1);
							value = getenv(var_name);
							free(var_name);
							if (value)
								len += strlen(value);
						}
					}
				}
				else
				{
					len++;
					i++;
				}
			}
			if (str[i] != '"')
				return ((size_t)-1);
			i++;
		}
		else if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				status_str = ft_itoa(shell->exit_status);
				if (!status_str)
					return ((size_t)-1);
				len += strlen(status_str);
				free(status_str);
				i += 2;
			}
			else
			{
				i++;
				var_start = i;
				while (str[i] && (isalnum((unsigned char)str[i])
						|| str[i] == '_'))
					i++;
				var_len = i - var_start;
				if (var_len == 0)
				{
					len++;
				}
				else
				{
					var_name = strndup(&str[var_start], var_len);
					if (!var_name)
						return ((size_t)-1);
					value = getenv(var_name);
					free(var_name);
					if (value)
						len += strlen(value);
				}
			}
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

/*
 * 第二パス: perform_expansion()
 * calculate_expanded_length() で求めた total_len のサイズのバッファを確保し、
 * 実際に展開処理を行いながら文字列を構築して返す。
 */
char	*perform_expansion(const char *str, t_shell *shell, size_t total_len)
{
	char	*out;
	size_t	out_index;
	size_t	i;
	size_t	t_len;
	size_t	v_len;

	out = malloc(total_len + 1);
	out_index = 0;
	i = 0;
	size_t var_start, var_len;
	char *var_name, *value, *temp_str;
	if (!out)
		return (NULL);
	while (str[i] != '\0')
	{
		if (str[i] == '\'')
		{
			i++;
			while (str[i] && str[i] != '\'')
				out[out_index++] = str[i++];
			if (str[i] != '\'')
			{
				free(out);
				return (NULL);
			}
			i++;
		}
		else if (str[i] == '"')
		{
			i++;
			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				{
					if (str[i + 1] == '?')
					{
						temp_str = ft_itoa(shell->exit_status);
						if (!temp_str)
						{
							free(out);
							return (NULL);
						}
						t_len = strlen(temp_str);
						memcpy(&out[out_index], temp_str, t_len);
						out_index += t_len;
						free(temp_str);
						i += 2;
						continue ;
					}
					else
					{
						i++;
						var_start = i;
						while (str[i] && (isalnum((unsigned char)str[i])
								|| str[i] == '_'))
							i++;
						var_len = i - var_start;
						if (var_len == 0)
							out[out_index++] = '$';
						else
						{
							var_name = strndup(&str[var_start], var_len);
							if (!var_name)
							{
								free(out);
								return (NULL);
							}
							value = getenv(var_name);
							free(var_name);
							if (!value)
								value = "";
							v_len = strlen(value);
							memcpy(&out[out_index], value, v_len);
							out_index += v_len;
						}
					}
				}
				else
				{
					out[out_index++] = str[i++];
				}
			}
			if (str[i] != '"')
			{
				free(out);
				return (NULL);
			}
			i++;
		}
		else if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				temp_str = ft_itoa(shell->exit_status);
				if (!temp_str)
				{
					free(out);
					return (NULL);
				}
				t_len = strlen(temp_str);
				memcpy(&out[out_index], temp_str, t_len);
				out_index += t_len;
				free(temp_str);
				i += 2;
			}
			else
			{
				i++;
				var_start = i;
				while (str[i] && (isalnum((unsigned char)str[i])
						|| str[i] == '_'))
					i++;
				var_len = i - var_start;
				if (var_len == 0)
				{
					out[out_index++] = '$';
				}
				else
				{
					var_name = strndup(&str[var_start], var_len);
					if (!var_name)
					{
						free(out);
						return (NULL);
					}
					value = getenv(var_name);
					free(var_name);
					if (!value)
						value = "";
					v_len = strlen(value);
					memcpy(&out[out_index], value, v_len);
					out_index += v_len;
				}
			}
		}
		else
		{
			out[out_index++] = str[i++];
		}
	}
	out[out_index] = '\0';
	return (out);
}

char	*expand(const char *str, t_shell *shell)
{
	int	total_len;

	total_len = expand_length(str, shell);
	if (total_len == -1)
	{
		shell->exit_status = 127;
		return (NULL);
	}
	return (perform_expansion(str, shell, total_len));
}

int	main(void)
{
	t_shell	shell;
	char	*input;
	char	*result;

	/* シェルの初期化: exit_status を設定（例として 0 を設定） */
	shell.exit_status = 0;
	/* テスト用の入力文字列（例） */
	input = "echo 'Hello, World' \"$HOME/Desktop\" $USER $?";
	/* expand() を呼び出して、展開後の文字列を取得 */
	result = expand(input, &shell);
	if (!result)
	{
		write(STDERR_FILENO, "Expansion error\n", 16);
		return (1);
	}
	/* 結果を表示 */
	printf("Original: %s\n", input);
	printf("Expanded: %s\n", result);
	free(result);
	return (0);
}
