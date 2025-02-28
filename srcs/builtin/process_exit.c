#include "../../include/minishell.h"

int	exec_exit(char **args, t_shell *shell)
{
	int	exit_code;

	if (shell->interactive)
		ft_putendl_fd("exit", STDOUT_FILENO);
	
	if (!args[1])
	{
		exit_code = shell->exit_status;
		exit(exit_code);
	}
	// 数値として解析できる場合
	if (ft_isdigit(args[1][0]) || args[1][0] == '-' || args[1][0] == '+')
	{
		exit_code = ft_atoi(args[1]);
		// 複数の引数がある場合はエラー
		if (args[2])
		{
			ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
			return (1);
		}
		exit(exit_code);
	}
	// 数値として解析できない場合
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	exit(255);
}
