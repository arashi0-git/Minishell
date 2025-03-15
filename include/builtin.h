/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:34:51 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/15 10:17:33 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "./minishell.h"

int		exec_export(char **cmd_and_args, t_shell *shell);
int		exec_cd(char **args, t_shell *shell);
int		exec_echo(char **args, t_shell *shell);
char	**sort_env(t_env *env);
void	free_array(char **sorted_env);
int		print_sorted_env(t_env *env);
int		exec_pwd(t_shell *shell);
int		exec_unset(char **args, t_shell *shell);
void	remove_env_unset(t_shell *shell, char *arg);
int		exec_env(char **args, t_shell *shell);
int		exec_exit(char **args, t_shell *shell);
int		exec_builtin(char **args, t_shell *shell);
int		is_builtin(char **args);
int		get_env_size(t_env *env);
int		process_export_argument(char *arg, t_shell *shell);
void	update_env_variable(char *args, t_env **env);

#endif
