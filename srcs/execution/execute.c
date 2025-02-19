

/* 基本的な実行フロー */
/*
 1.コマンドの種類を判別（ビルトインかバイナリか）

//コマンドの種類を判別する関数
int execute_commond(t_cmd *commond)
{
    if (is_builtin(commond->cmd))
        return (exec_builtin);

    return execute_external(commond);
}

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <errno.h>
// readline関連のヘッダー
#include <readline/readline.h>
#include <readline/history.h>

// environの宣言
extern char **environ;  // または __environ を使用

#define MAX_COMMAND 256
#define MAX_ARGS 100

// シグナル用グローバル変数（要件による）
volatile sig_atomic_t g_signal;

typedef struct s_redirect {
    char    *infile;        // 入力リダイレクト用ファイル
    char    *outfile;       // 出力リダイレクト用ファイル
    bool    append;         // >> モード
    char    *heredoc;       // heredocのデリミタ
    int     heredoc_fd;     // heredoc用のパイプfd
} t_redirect;

typedef struct s_command {
    char        *cmd;           // コマンド名
    char        **args;         // 引数配列
    t_redirect  redirect;       // リダイレクション情報
    bool        pipe_next;      // パイプの有無
} t_command;

// プロトタイプ宣言
void    init_command(t_command *cmd);
int     execute_commands(t_command *commands, int cmd_count, char **envp);
int     setup_redirects(t_command *cmd);
void    restore_redirects(int stdin_fd, int stdout_fd);
char    *find_command_path(const char *cmd, char **envp);
int     handle_heredoc(t_command *cmd);

// コマンド初期化
void init_command(t_command *cmd)
{
    cmd->cmd = NULL;
    cmd->args = NULL;
    cmd->redirect.infile = NULL;
    cmd->redirect.outfile = NULL;
    cmd->redirect.append = false;
    cmd->redirect.heredoc = NULL;
    cmd->redirect.heredoc_fd = -1;
    cmd->pipe_next = false;
}

// リダイレクション設定
int setup_redirects(t_command *cmd)
{
    int stdin_backup = -1;
    int stdout_backup = -1;

    // 標準入出力のバックアップ
    if (cmd->redirect.infile || cmd->redirect.heredoc) {
        stdin_backup = dup(STDIN_FILENO);
    }
    if (cmd->redirect.outfile) {
        stdout_backup = dup(STDOUT_FILENO);
    }

    // 入力リダイレクト
    if (cmd->redirect.heredoc && cmd->redirect.heredoc_fd != -1) {
        dup2(cmd->redirect.heredoc_fd, STDIN_FILENO);
        close(cmd->redirect.heredoc_fd);
    }
    else if (cmd->redirect.infile) {
        int fd = open(cmd->redirect.infile, O_RDONLY);
        if (fd == -1) {
            perror("open");
            return -1;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    // 出力リダイレクト
    if (cmd->redirect.outfile) {
        int flags = O_WRONLY | O_CREAT;
        flags |= (cmd->redirect.append) ? O_APPEND : O_TRUNC;
        int fd = open(cmd->redirect.outfile, flags, 0644);
        if (fd == -1) {
            perror("open");
            return -1;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    return 0;
}

// 複数コマンドの実行（多段パイプ対応）
int execute_commands(t_command *commands, int cmd_count, char **envp)
{
    int prev_pipe[2] = {-1, -1};
    int curr_pipe[2];
    int i = 0;
    int status = 0;
    pid_t last_pid = -1;

    while (i < cmd_count) {
        // 次のコマンドへのパイプを作成
        if (commands[i].pipe_next) {
            if (pipe(curr_pipe) == -1) {
                perror("pipe");
                return 1;
            }
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            // 子プロセス
            // 入力パイプのせってい
            if (prev_pipe[0] != -1) {
                dup2(prev_pipe[0], STDIN_FILENO);
                close(prev_pipe[0]);
                close(prev_pipe[1]);
            }

            // 出力パイプの設定
            if (commands[i].pipe_next) {
                close(curr_pipe[0]);
                dup2(curr_pipe[1], STDOUT_FILENO);
                close(curr_pipe[1]);
            }

            // リダイレクション
            if (setup_redirects(&commands[i]) == -1)
                exit(1);

            // コマンドの実行
            char *cmd_path = find_command_path(commands[i].cmd, envp);
            if (!cmd_path) {
                fprintf(stderr, "command not found: %s\n", commands[i].cmd);
                exit(127);
            }
            execve(cmd_path, commands[i].args, envp);
            perror("execve");
            exit(1);
        }

        // 親プロセス
        // 前のパイプをクローズ
        if (prev_pipe[0] != -1) {
            close(prev_pipe[0]);
            close(prev_pipe[1]);
        }

        // 現在のパイプを前のパイプとして保存
        if (commands[i].pipe_next) {
            prev_pipe[0] = curr_pipe[0];
            prev_pipe[1] = curr_pipe[1];
        } else {
            prev_pipe[0] = prev_pipe[1] = -1;
        }

        last_pid = pid;
        i++;
    }

    // 最後のコマンドの終了を待つ
    waitpid(last_pid, &status, 0);
    while (wait(NULL) > 0)
        ;  // 他の子プロセスの終了を待つ

    return WEXITSTATUS(status);
}

// Heredocの処理
int handle_heredoc(t_command *cmd)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        return -1;
    }

    // heredocの内容を読み込んでパイプに書き込む
    while (1) {
        line = readline("> ");
        if (!line || strcmp(line, cmd->redirect.heredoc) == 0) {
            free(line);
            break;
        }
        write(pipe_fd[1], line, strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }

    close(pipe_fd[1]);
    return pipe_fd[0];
}

// メイン処理（テスト用）
int main(void)
{
    // テストケース
    t_command commands[4];
    char **envp = environ;  // 環境変数

    // コマンド1: ls -l
    init_command(&commands[0]);
    commands[0].cmd = strdup("ls");
    commands[0].args = malloc(sizeof(char *) * 3);
    commands[0].args[0] = strdup("ls");
    commands[0].args[1] = strdup("-l");
    commands[0].args[2] = NULL;
    commands[0].pipe_next = true;

    // コマンド2: grep .c
    init_command(&commands[1]);
    commands[1].cmd = strdup("grep");
    commands[1].args = malloc(sizeof(char *) * 3);
    commands[1].args[0] = strdup("grep");
    commands[1].args[1] = strdup(".c");
    commands[1].args[2] = NULL;
    commands[1].pipe_next = true;

    // コマンド3: wc -l
    init_command(&commands[2]);
    commands[2].cmd = strdup("wc");
    commands[2].args = malloc(sizeof(char *) * 3);
    commands[2].args[0] = strdup("wc");
    commands[2].args[1] = strdup("-l");
    commands[2].args[2] = NULL;
    commands[2].pipe_next = false;

    // 実行
    int status = execute_commands(commands, 3, envp);
    printf("Exit status: %d\n", status);

    // メモリ解放（実際の実装では必要）
    // ...
     printf("=== Test 2: Heredoc (cat << EOF) ===\n");
    printf("Enter text (type 'EOF' to end):\n");
    
    // コマンド4: heredocテスト
    init_command(&commands[3]);
    commands[3].cmd = strdup("cat");
    commands[3].args = malloc(sizeof(char *) * 2);
    commands[3].args[0] = strdup("cat");
    commands[3].args[1] = NULL;
    commands[3].redirect.heredoc = strdup("EOF");
    commands[3].pipe_next = false;

    // heredocの設定
    commands[3].redirect.heredoc_fd = handle_heredoc(&commands[3]);
    if (commands[3].redirect.heredoc_fd == -1) {
        fprintf(stderr, "heredoc setup failed\n");
        return 1;
    }

    // heredocの実行
    status = execute_commands(&commands[3], 1, envp);
    printf("Exit status: %d\n", status);

    // メモリ解放（heredoc用）
    free(commands[3].cmd);
    free(commands[3].args[0]);
    free(commands[3].args);
    free(commands[3].redirect.heredoc);
    return 0;
}

