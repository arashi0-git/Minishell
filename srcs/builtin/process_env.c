#include "../../include/minishell.h"

int	exec_env(char **args, t_shell *shell)
{
	t_env	*env;

	// 引数がある場合はエラー（単純な実装）
	if (args[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		return (1);
	}
	// 環境変数を表示
	env = shell->env;
	while (env)
	{
		// 値がある場合のみ表示
		if (env->value)
		{
			ft_putstr_fd(env->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putendl_fd(env->value, STDOUT_FILENO);
		}
		env = env->next;
	}
	return (0);
}
