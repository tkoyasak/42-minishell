/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:05:05 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/29 19:23:25 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "struct.h"
# include "executor.h"

int		builtin_echo(t_proc *proc, t_sh_var *sh_var);
int		builtin_cd(t_proc *proc, t_sh_var *sh_var);
void	update_relative_path(char *old_pwd, char *path_name, \
											 t_sh_var *sh_var);
void	update_absolute_path(char *path_name, t_sh_var *sh_var);
int		builtin_pwd(t_proc *proc, t_sh_var *sh_var);
int		builtin_export(t_proc *proc, t_sh_var *sh_var);
int		builtin_unset(t_proc *proc, t_sh_var *sh_var);
int		builtin_env(t_proc *proc, t_sh_var *sh_var);
int		builtin_exit(t_proc *proc, t_sh_var *sh_var);

#endif
