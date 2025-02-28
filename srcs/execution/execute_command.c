/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/28 17:27:57 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/execution.h"
#include "../../include/redirect.h"

// executor_command
/*
コマンド実行のメインエントリーポイント
重要な処理フロー:

コマンドトークンを実行可能な形式に変換
パイプがなく、かつビルトインコマンドの場合は親プロセスで直接実行
それ以外の場合は子プロセスを作成して実行
リダイレクトのクリーンアップとパイプ状態の更新
*/

static int	exec_builtin_parent(t_shell *shell, t_cmd *command, char **args)
{
	if (setup_redirects(command) == FALSE)
		return (EXIT_FAILURE);
	if (dup_redirects(command, TRUE) == FALSE)
		return (EXIT_FAILURE);
	return (exec_builtin(args, shell));
}

static void	execute_in_child(t_shell *shell, t_cmd *cmd, t_pipe_state state,
		int old_pipe[2], int new_pipe[2])
{
	if (!setup_redirects(cmd))
		exit(EXIT_FAILURE);
	setup_pipes(state, old_pipe, new_pipe);
	if (is_builtin(&cmd->args[0]))
		exit(exec_builtin(cmd->args, shell));
	else
		exec_binary(shell, cmd->args);
}

int	execute_command(t_shell *shell, t_cmd *cmd)
{
	t_pipe_state	state;
	int				status;
	int				old_pipe[2] = {-1, -1};
	int				new_pipe[2] = {-1, -1};
	pid_t			pid;

	init_pipe_state(&state, cmd);
	expand_cmd(cmd, shell);
	create_pipe(state, new_pipe);
	if (state == NO_PIPE && is_builtin(&cmd->args[0]))
		return (exec_builtin_parent(shell, cmd, cmd->args));
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
		execute_in_child(shell, cmd, state, old_pipe, new_pipe);
	cleanup_pipe(state, old_pipe, new_pipe);
	cmd->pid = pid;
	if (state == NO_PIPE || state == PIPE_READ_ONLY)
	{
		// 単一コマンドまたはパイプラインの最後のコマンドの場合は待機
		status = wait_for_command(pid);
		shell->exit_status = status;
		return (status);
	}
	else
	{
		// パイプラインの先頭または中間の場合は待機せずに返す
		return (1);
	}
}
