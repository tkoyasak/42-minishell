/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 14:05:05 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 14:36:12 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "struct.h"
# include "executor.h"

int		builtin_echo(t_proc *proc, t_sh_var *sh_var);
int		builtin_cd(t_proc *proc, t_sh_var *sh_var);
int		builtin_pwd(t_proc *proc, t_sh_var *sh_var);
int		builtin_export(t_proc *proc, t_sh_var *sh_var);
int		builtin_unset(t_proc *proc, t_sh_var *sh_var);
int		builtin_env(t_proc *proc, t_sh_var *sh_var);
int		builtin_exit(t_proc *proc, t_sh_var *sh_var);

#endif
