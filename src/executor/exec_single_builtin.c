/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/08 16:39:39 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 16:40:54 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  execute single builtin  */
int	exec_single_builtin(t_expr *expr, t_sh_var *sh_var)
{
	t_list		*proc_list;
	t_proc		*proc;
	int			res;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	set_command(proc);
	res = set_io_params(proc, sh_var);
	dup2_func(expr, proc, 0);
	close_func(expr, proc, 0);
	if (res)
	{
		g_exit_status = 1;
		return (g_exit_status);
	}
	return (exec_builtin(proc, sh_var));
}
