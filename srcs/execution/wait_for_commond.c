#include "../../include/minishell.h"
#include <sys/wait.h>

/*
 * 子プロセスの終了を待ち、終了ステータスを返す関数
 */
int	wait_for_command(pid_t pid)
{
	int	status;

	// プロセスIDが無効な場合
	if (pid <= 0)
		return (0);
	// 指定されたプロセスの終了を待つ
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	// 終了ステータスを返す
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (EXIT_FAILURE);
}
