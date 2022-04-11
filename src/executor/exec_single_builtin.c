/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 16:39:39 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/11 12:23:57 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	recover_stdio(int fd[], int backup[])
{
	if (fd[0] != FD_NONE)
	{
		safe_func(dup2(backup[0], STDIN_FILENO));
		safe_func(close(backup[0]));
	}
	if (fd[1] != FD_NONE)
	{
		safe_func(dup2(backup[1], STDOUT_FILENO));
		safe_func(close(backup[1]));
	}
}

/*  execute single builtin  */
int	exec_single_builtin(t_expr *expr, t_sh_var *sh_var)
{
	int			backup[2];
	t_list		*proc_list;
	t_proc		*proc;
	int			res;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	set_command(proc);
	res = set_io_params(proc, sh_var);
	if (proc->fd[0] != FD_NONE)
		backup[0] = safe_func(dup(STDIN_FILENO));
	if (proc->fd[1] != FD_NONE)
		backup[1] = safe_func(dup(STDOUT_FILENO));
	dup2_func(expr, proc, 0);
	close_func(expr, proc, 0);
	if (res)
	{
		g_exit_status = 1;
		recover_stdio(proc->fd, backup);
		return (g_exit_status);
	}
	res = exec_builtin(proc, sh_var);
	recover_stdio(proc->fd, backup);
	return (res);
}
