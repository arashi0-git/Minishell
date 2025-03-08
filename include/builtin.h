/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aryamamo <aryamamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:27:26 by aryamamo          #+#    #+#             */
/*   Updated: 2025/03/08 20:27:26 by aryamamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H
# include "./minishell.h"

int		exec_export(char **cmd_and_args, t_shell *shell);
int		exec_cd(char **args, t_shell *shell);
int		exec_echo(char **args);
char	**sort_env(t_env *env);
void	free_array(char **sorted_env);
int		print_sorted_env(t_env *env);
int		exec_pwd(void);
int		exec_unset(char **args, t_shell *shell);
void	remove_env_unset(t_shell *shell, char *arg);
int		exec_env(char **args, t_shell *shell);
// int		exec_exit(char **args);
int		exec_builtin(char **args, t_shell *shell);
int		is_builtin(char **args);
int		get_env_size(t_env *env);

#endif
