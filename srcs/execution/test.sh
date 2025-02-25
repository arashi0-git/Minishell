#!/bin/bash

# バックアップディレクトリの作成
backup_dir="backup_$(date +%Y%m%d_%H%M%S)"
mkdir "${backup_dir}"

# 現在のディレクトリのファイルをバックアップ
cp *.c "${backup_dir}/"
cp ../../include/*.h "${backup_dir}/"

# t_command から t_cmd への置換
# 現在のディレクトリ（.c files）
sed -i'.bak' 's/t_command/t_cmd/g' *.c

# includeディレクトリ（.h files）
sed -i'.bak' 's/t_command/t_cmd/g' ../../include/*.h

# .bakファイルの削除
find . -name "*.bak" -delete
find ../../include -name "*.bak" -delete

echo "変換が完了しました。バックアップは ${backup_dir} に保存されています。"

