/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_commond_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retoriya <retoriya@student.42tokyo.jp      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:58:24 by retoriya          #+#    #+#             */
/*   Updated: 2025/02/28 18:04:41 by retoriya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
create_path: 2つのパス文字列（ディレクトリとコマンド）を結合する関数

	例：dir="/usr/bin" + cmd="ls" → "/usr/bin/ls"
	メモリを動的に確保し、新しいパス文字列を作成
	スラッシュ(/)を自動的に間に挿入
*/

static char	*create_path(const char *dir, const char *cmd)
{
	char	*path;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!path)
		return (NULL);
	while (dir[i])
	{
		path[i] = dir[i];
		i++;
	}
	path[i++] = '/';
	while (cmd[j])
		path[i++] = cmd[j++];
	path[i] = '\0';
	return (path);
}

/*
get_env_path...環境変数からPATH情報を取得する関数

envp配列から "PATH=" で始まる文字列を探す
見つかったらPATH=以降の文字列のポインタを返す
例：PATH=/usr/bin:/bin:/usr/local/bin
*/

static char	*get_env_path(char **envp)
{
	while (*envp)
	{
		if (ft_strncmp(*envp, "PATH=", 5) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

/*
check_dir_path...特定のディレクトリ内でコマンドが実行可能かチェックする関数

パスの区切り文字(:)を一時的にNULL文字に置き換えて処理
create_pathで完全なパスを作成
accessでそのパスが実行可能かチェック
実行可能なら、そのパスを返す。不可能ならNULLを返す
*/

static char	*check_dir_path(char *dir_start, const char *cmd)
{
	char	*temp_path;
	int		j;

	j = 0;
	while (dir_start[j] && dir_start[j] != ':')
		j++;
	if (dir_start[j])
		dir_start[j] = '\0';
	temp_path = create_path(dir_start, cmd);
	dir_start[j] = ':';
	if (temp_path && access(temp_path, X_OK) == 0)
		return (temp_path);
	free(temp_path);
	return (NULL);
}

/*
 find_commmond_path...以下の順序で実行可能なパスを探索：

コマンドが絶対パス(/で始まる)か相対パス(./で始まる)の場合

そのパスが実行可能かを直接チェック


それ以外の場合（コマンド名のみの場合）

環境変数PATHから検索パスを取得
各ディレクトリでコマンドを探索
最初に見つかった実行可能なパスを返す
*/
char	*find_command_path(const char *cmd, char **envp)
{
	char	*path;
	char	*dir_start;
	char	*temp_path;
	int		j;

	if (cmd[0] == '/' || cmd[0] == '.')
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
	path = get_env_path(envp);
	if (!path)
		return (NULL);
	dir_start = path;
	while (*dir_start)
	{
		if ((temp_path = check_dir_path(dir_start, cmd)))
			return (temp_path);
		j = 0;
		while (dir_start[j] && dir_start[j] != ':')
			j++;
		dir_start = dir_start + j;
		if (*dir_start)
			dir_start++;
	}
	return (NULL);
}
