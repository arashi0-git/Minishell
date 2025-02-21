typedef enum e_pipe_state {
    NO_PIPE,
    PIPE_READ_ONLY,
    PIPE_WRITE_ONLY,
    PIPE_READ_WRITE
} t_pipe_state;

typedef struct s_command {
    char            **args;         
    struct s_redirect *redirects;   
    struct s_command *next;
    pid_t           pid;
} t_command;

#define PIPE_IN     0
#define PIPE_OUT    1

// executor_command
/*
コマンド実行のメインエントリーポイント
重要な処理フロー:

コマンドトークンを実行可能な形式に変換
パイプがなく、かつビルトインコマンドの場合は親プロセスで直接実行
それ以外の場合は子プロセスを作成して実行
リダイレクトのクリーンアップとパイプ状態の更新
*/

static int exec_builtin_parent(t_shell *shell, t_command *command, char **args)
{
    if (setup_redirects(command) == FALSE)
        return (EXIT_FAILURE);
    if (dup_redirects(command, TRUE) == FALSE)
        return (EXIT_FAILURE);
    return (exec_builtin(shell, args));
}

static void execute_in_child(t_shell *shell, t_command *cmd, 
                           t_pipe_state state, int old_pipe[2], int new_pipe[2])
{
    if (!setup_redirects(cmd))
        exit(EXIT_FAILURE);

    setup_pipes(state, old_pipe, new_pipe);

    if (is_builtin(cmd->args[0]))
        exit(exec_builtin(shell, cmd->args));
    else
        exec_binary(shell, cmd->args);
}

int execute_command(t_shell *shell, t_command *cmd)
{
    t_pipe_state state;
    int old_pipe[2] = {-1, -1};
    int new_pipe[2] = {-1, -1};
    pid_t pid;

    init_pipe_state(&state, cmd);
    expand_cmd(cmd, shell);

    create_pipe(state, new_pipe);

    if (state == NO_PIPE && is_builtin(cmd->args[0]))
        return exec_builtin_parent(shell, cmd, cmd->args);

    pid = fork();
    if (pid < 0)
        return EXIT_FAILURE;

    if (pid == 0)
        execute_in_child(shell, cmd, state, old_pipe, new_pipe);

    cleanup_pipe(state, old_pipe, new_pipe);
    cmd->pid = pid;
    return wait_for_command(pid);
}

