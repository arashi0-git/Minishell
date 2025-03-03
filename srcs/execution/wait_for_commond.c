#include "../../include/minishell.h"
#include <sys/wait.h>

/*
 * 子プロセスの終了を待ち、終了ステータスを返す関数
 */
int	wait_for_command(pid_t pid)
{
	int	status;
	int	sig;

	// プロセスIDが無効な場合
	if (pid <= 0)
		return (0);
	// 指定されたプロセスの終了を待つ
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return (EXIT_FAILURE);
	}
	if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		if (sig == SIGQUIT)
			write(STDOUT_FILENO, "Quit (core dumped)\n", 20);
		return (128 + sig);
	}
	// 終了ステータスを返す
	else if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
