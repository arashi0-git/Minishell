/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 21:12:30 by retoriya          #+#    #+#             */
/*   Updated: 2025/03/08 20:23:02 by retoriya         ###   ########.fr       */
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

void handle_command_file_args(t_cmd *cmd)
{
    if (!cmd->args[0] || !cmd->args[1] || cmd->infile)
        return;
    
    if (ft_strcmp(cmd->args[0], "cat") != 0)
        return;
    
    t_token *token = malloc(sizeof(t_token));
    if (!token)
        return;
    
    token->value = ft_strdup(cmd->args[1]);
    token->type = TOKEN_COMMAND;
    token->next = NULL;
    
    t_redirect *redir = create_redirect(REDIRECT_IN, token, STDIN_FILENO);
    if (!redir || !check_redirect(redir))
    {
        if (redir)
            free_redirect(redir);
        else
            free_token_list(token);
        return;
    }
    
    redir->next = (t_redirect *)cmd->redirects;
    cmd->redirects = redir;
}


static int	exec_builtin_parent(t_shell *shell, t_cmd *command, char **args)
{
	int	result;

	result = 0;
	if (setup_redirects(command) == FALSE)
		return (EXIT_FAILURE);
    fprintf(stderr, "Debug: Before dup_redirects\n");
	if (dup_redirects(command, TRUE) == FALSE)
		return (EXIT_FAILURE);
    fprintf(stderr, "Debug: After dup_redirects\n");
	result = exec_builtin(args, shell);
	cleanup_redirects(command);
	return (result);
}

/*
static void	execute_in_child(t_shell *shell, t_cmd *cmd, t_pipe_state state,
		int old_pipe[2], int new_pipe[2])
{
	reset_signal_in_child();
    handle_command_file_args(cmd);
     // リダイレクトリストの内容をデバッグ出力
    fprintf(stderr, "Debug: Redirect list after handle_command_file_args:\n");
    t_redirect *debug_redir = (t_redirect *)cmd->redirects;
    while (debug_redir) {
        fprintf(stderr, "  Type: %d, fd_io: %d, fd_file: %d, filename: %s\n", 
                debug_redir->type, debug_redir->fd_io, debug_redir->fd_file,
                debug_redir->filename ? debug_redir->filename->value : "(null)");
        debug_redir = debug_redir->next;
    }
	if (!setup_redirects(cmd))
		exit(EXIT_FAILURE);
	setup_pipes(state, old_pipe, new_pipe);
	if (is_builtin(&cmd->args[0]))
		exit(exec_builtin(cmd->args, shell));
	else
		exec_binary(shell, cmd->args);
}
*/

static void execute_in_child(t_shell *shell, t_cmd *cmd, t_pipe_state state,
        int old_pipe[2], int new_pipe[2])
{
    reset_signal_in_child();
    
    // 他のケースは通常の実行を続行
    handle_command_file_args(cmd);
    if (!setup_redirects(cmd))
        exit(EXIT_FAILURE);

    if (!dup_redirects(cmd, FALSE))
        exit(EXIT_FAILURE);
    
    setup_pipes(state, old_pipe, new_pipe);
    if (is_builtin(&cmd->args[0]))
        exit(exec_builtin(cmd->args, shell));
    else
        exec_binary(shell, cmd->args);
}

int	finalize_command(t_shell *shell, t_pipe_state state, pid_t pid,
		struct sigaction *old_sa)
{
	int	status;

	if (state == NO_PIPE || state == PIPE_READ_ONLY)
	{
		status = wait_for_command(pid);
		shell->exit_status = status;
		sigaction(SIGINT, old_sa, NULL);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);
		return (status);
	}
	return (1);
}

void	cleanup_pipe_ext(t_pipe_state state, int shared_pipe[2],
		int new_pipe[2])
{
	// READ_ONLYまたはREAD_WRITEの場合、古いパイプはもう不要なので閉じる
	if (state == PIPE_READ_ONLY || state == PIPE_READ_WRITE)
	{
		if (shared_pipe[PIPE_IN] != -1)
			close(shared_pipe[PIPE_IN]);
		if (shared_pipe[PIPE_OUT] != -1)
			close(shared_pipe[PIPE_OUT]);
	}
	// 新しいパイプの書き込み側を閉じる
	if (state == PIPE_WRITE_ONLY || state == PIPE_READ_WRITE)
	{
		if (new_pipe[PIPE_OUT] != -1)
			close(new_pipe[PIPE_OUT]);
		// 次のコマンドのために情報を保存
		shared_pipe[PIPE_IN] = new_pipe[PIPE_IN];
		shared_pipe[PIPE_OUT] = -1;
	}
}


int	execute_command(t_shell *shell, t_cmd *cmd, t_pipe_state state,
		int shared_pipe[2])
{
	int		new_pipe[2] = {-1, -1};
	pid_t	pid;

	// int status;
	struct sigaction new_sa, old_sa;
	create_pipe(state, new_pipe);
	if (state == NO_PIPE && is_builtin(&cmd->args[0]))
		return (exec_builtin_parent(shell, cmd, cmd->args));
	pid = fork();
	if (pid < 0)
		return (EXIT_FAILURE);
	if (pid == 0)
		execute_in_child(shell, cmd, state, shared_pipe, new_pipe);
	new_sa.sa_handler = SIG_IGN;
	sigemptyset(&new_sa.sa_mask);
	new_sa.sa_flags = 0;
	sigaction(SIGINT, &new_sa, &old_sa);
	cleanup_pipe_ext(state, shared_pipe, new_pipe);
	cmd->pid = pid;
	return (finalize_command(shell, state, pid, &old_sa));
}

/*
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
*/
