/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/03 16:47:02 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execution.h"
#include "../../include/expand.h"
#include "../../include/parse.h"
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

void	reset_signal_in_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
	sa.sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &sa, NULL) < 0)
	{
		perror("sigaction");
		exit(1);
	}
}

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
	reset_signal_in_child();
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
	t_pipe_state		state;
	int					status;
	int					old_pipe[2] = {-1, -1};
	int					new_pipe[2] = {-1, -1};
	pid_t				pid;
	struct sigaction	new_sa;
	struct sigaction	old_sa;

	init_pipe_state(&state, cmd);
	create_pipe(state, new_pipe);
	if (state == NO_PIPE && is_builtin(&cmd->args[0]))
		return (exec_builtin_parent(shell, cmd, cmd->args));
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
		execute_in_child(shell, cmd, state, old_pipe, new_pipe);
	new_sa.sa_handler = SIG_IGN;
	sigemptyset(&new_sa.sa_mask);
	new_sa.sa_flags = 0;
	sigaction(SIGINT, &new_sa, &old_sa);
	cleanup_pipe(state, old_pipe, new_pipe);
	cmd->pid = pid;
	if (state == NO_PIPE || state == PIPE_READ_ONLY)
	{
		status = wait_for_command(pid);
		shell->exit_status = status;
		// 待機終了後、元のSIGINTハンドラに戻す
		sigaction(SIGINT, &old_sa, NULL);
		// SIGINTにより子プロセスが終了した場合は改行を出力する
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (status);
	}
	else
	{
		// パイプラインの先頭または中間の場合は待機せずに返す
		return (1);
	}
}
