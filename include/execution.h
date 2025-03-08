#ifndef EXECUTION_H
# define EXECUTION_H

# include "./minishell.h"

# define STATUS_CMD_NOT_FOUND 127
# define STATUS_CMD_NOT_EXECUTABLE 126
# define PIPE_IN 0
# define PIPE_OUT 1

typedef enum e_pipe_state
{
	NO_PIPE,
	PIPE_READ_ONLY,
	PIPE_WRITE_ONLY,
	PIPE_READ_WRITE
}		t_pipe_state;

int		execute_command(t_shell *shell, t_cmd *cmd, t_pipe_state state, int shared_pipe[2]);
void	exec_binary(t_shell *shell, char **args);
void	handle_execve_error(char *path);
char	**create_environ(t_env *env);
t_bool	is_directory(const char *path);

void	create_pipe(t_pipe_state state, int new_pipe[]);
void	setup_pipes(t_pipe_state state, int old_pipe[2], int new_pipe[2]);
void	init_pipe_state(t_pipe_state *state, t_cmd *cmd);
void	cleanup_pipe(t_pipe_state state, int old_pipe[2], int new_pipe[2]);

char	*find_command_path(const char *cmd, char **envp);

size_t	get_environ_size(t_env *envs);
t_bool	can_generate_environ(t_env *env);
int		wait_for_command(pid_t pid);

void handle_command_file_args(t_cmd *cmd);
void	process_input(t_shell *shell, char *input);
void	free_cmd_list(t_cmd *cmd_list);
#endif
