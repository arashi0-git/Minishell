/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 13:03:31 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/11 20:41:00 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_env_node(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}

void	remove_env_unset(t_shell *shell, char *arg)
{
	t_env	*current;
	t_env	*prev;

	if (ft_strcmp(shell->env->key, arg) == 0)
	{
		current = shell->env;
		shell->env = shell->env->next;
		free_env_node(current);
		return ;
	}
	prev = shell->env;
	current = shell->env->next;
	while (current && ft_strcmp(current->key, arg) != 0)
	{
		prev = current;
		current = current->next;
	}
	if (current)
	{
		prev->next = current->next;
		free_env_node(current);
		return ;
	}
}

int	exec_unset(char **args, t_shell *shell)
{
	int	i;
	int	status;

	if (!args[1])
		return (0); // 引数がない場合は成功として扱う
	i = 1;
	status = 0;
	if (!shell->env)
		return ;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			status = 0;
		}
		else
			remove_env_unset(shell, args[i]);
		i++;
	}
	return (status);
}
/*cc -Wall -Wextra -Werror -g  process_unset.c ../env/env.c -I../../include -L../../libft -lft
 //
static t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->env = NULL;
	// 基本的な環境変数を設定
	set_env(&shell->env, "C", "/home/user");
	set_env(&shell->env, "B", "/usr/bin");
	set_env(&shell->env, "A", "/usr/local");
	return (shell);
}

static void	print_env(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

// シェル構造体の解放関数 - main関数の外で定義
static void	free_shell(t_shell *shell)
{
	t_env	*current;
	t_env	*next;

	if (!shell)
		return ;
	// 環境変数リストの解放
	current = shell->env;
	while (current)
	{
		next = current->next;
		free_env_node(current);
		current = next;
	}
	free(shell);
}

int	is_valid_identifier(const char *str)
{
	char	*equal_pos;
	size_t	key_length;
	size_t	i;

	equal_pos = NULL;
	i = 0;
	if (!str || !*str)
		return (0);
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	equal_pos = ft_strchr(str, '=');
	if (equal_pos)
		key_length = equal_pos - str;
	else
		key_length = ft_strlen(str);
	i = 1;
	while (i < key_length)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}


int	main(void)
{
	t_shell	*shell;
	char	*test_args[5];
	int		status;

	shell = init_shell();
	if (!shell)
		return (1);
	// 初期状態の環境変数を表示
	printf("=== Initial Environment ===\n");
	print_env(shell->env);
	printf("\n");
	// テストケース1: 引数なしの場合
	printf("=== Test 1: unset without args ===\n");
	test_args[0] = "unset";
	test_args[1] = NULL;
	status = exec_unset(test_args, shell);
	printf("Status: %d\n", status);
	printf("Environment after test 1:\n");
	print_env(shell->env);
	printf("\n");
	// テストケース2: 存在する変数の削除
	printf("=== Test 2: unset existing variable ===\n");
	test_args[0] = "unset";
	test_args[1] = "B";
	test_args[2] = NULL;
	status = exec_unset(test_args, shell);
	printf("Status: %d\n", status);
	printf("Environment after unset B:\n");
	print_env(shell->env);
	printf("\n");
	// テストケース3: 存在しない変数の削除
	printf("=== Test 3: unset non-existing variable ===\n");
	test_args[0] = "unset";
	test_args[1] = "NONEXISTENT";
	test_args[2] = NULL;
	status = exec_unset(test_args, shell);
	printf("Status: %d\n", status);
	printf("Environment after test 3:\n");
	print_env(shell->env);
	printf("\n");
	// テストケース4: 複数変数の削除
	printf("=== Test 4: unset multiple variables ===\n");
	test_args[0] = "unset";
	test_args[1] = "A";
	test_args[2] = "C";
	test_args[3] = NULL;
	status = exec_unset(test_args, shell);
	printf("Status: %d\n", status);
	printf("Environment after multiple unset:\n");
	print_env(shell->env);
	printf("\n");
	// テストケース5: 不正な識別子
	printf("=== Test 5: unset invalid identifier ===\n");
	test_args[0] = "unset";
	test_args[1] = "2INVALID";
	test_args[2] = NULL;
	status = exec_unset(test_args, shell);
	printf("Status: %d\n", status);
	printf("Environment after test 5:\n");
	print_env(shell->env);
	printf("\n");
	// シェルのクリーンアップ
	free_shell(shell);
	return (0);
}
*/
