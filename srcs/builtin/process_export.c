/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/09 15:18:21 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/04 19:16:21 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	update_env_variable(char *args, t_env **env)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	char	*args_copy;

	args_copy = ft_strdup(args);
	if (!args || !env)
		return ;
	equal_pos = ft_strchr(args_copy, '=');
	if (equal_pos)
	{
		*equal_pos = '\0';
		key = args_copy;
		value = equal_pos + 1;
		set_env(env, key, value);
	}
	else
		set_env(env, args, "");
	free(args_copy);
}

int	exec_export(char **cmd_and_args, t_shell *shell)
{
	int	i;

	if (!cmd_and_args[1])
		return (print_sorted_env(shell->env));
	i = 1;
	while (cmd_and_args[i])
	{
		if (is_valid_identifier(cmd_and_args[i]))
			update_env_variable(cmd_and_args[i], &shell->env);
		i++;
	}
	return (0);
}

/*
// #compile option:libftをmake &&  cc -Wall -Wextra -Werror
	-g  process_export.c process_export_utils.c process_export_utils2.c ../env/env.c
	-I../../include -L../../libft -lft
void	clean_env(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

void	clean_shell(t_shell *shell)
{
	if (!shell)
		return ;
	clean_env(shell->env);
	free(shell);
}

#include <stdio.h>

// 簡単なテスト用のシェル環境を作成
static t_shell	*init_test_shell(void)
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

int	main(void)
{
	t_shell	*shell;
	char	*test_args[3];

	shell = init_test_shell();
	if (!shell)
		return (1);
	printf("=== Test 1: export without args ===\n");
	test_args[0] = "export";
	test_args[1] = NULL;
	exec_export(test_args, shell);
	printf("\n");
	printf("=== Test 2: export with new variable ===\n");
	test_args[0] = "export";
	test_args[1] = "TEST_VAR=hello";
	test_args[2] = NULL;
	exec_export(test_args, shell);
	// 結果を確認するため再度表示
	test_args[1] = NULL;
	exec_export(test_args, shell);
	clean_shell(shell);
	return (0);
}
*/
/*
   export [-fn] [name[=word]] ...
		export -p
				The  supplied names are marked for automatic export to the environment of subse‐
				quently executed commands.  If the -f option is given,
					the names refer to  func‐
				tions.   If no names are given, or if the -p option is supplied,
					a list of names
				of all exported variables is printed.  The
					-n option causes the export  property
				to  be  removed  from  each  name.  If a variable name is followed by =word,
					the
				value of the variable is set to word.  export returns an exit status of 0 unless
				an invalid option is encountered,
					one of the names is not a valid shell variable
				name, or -f is supplied with a name that is not a function.


	#example
		export MY_VAR="hello"
	# この後に実行される全てのコマンドで
	# MY_VARという環境変数が利用可能になる

	bash  # 新しいシェルを起動
	echo $MY_VAR  # "hello"が表示される
	→子プロセスに継承されるため
*/
