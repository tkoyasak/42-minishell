/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:16:41 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 15:21:17 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  execute single child process  */
static void	exec_single_child(t_expr *expr, t_proc *proc, int cmd_idx, \
															t_sh_var *sh_var)
{
	char		*fullpath_cmd;
	struct stat	buf;

	xsignal(SIGINT, SIG_DFL);
	xsignal(SIGQUIT, SIG_DFL);
	dup2_func(expr, proc, cmd_idx);
	close_func(expr, proc, cmd_idx);
	fullpath_cmd = get_fullpath_cmd(proc->command[0], sh_var);
	if (fullpath_cmd)
	{
		safe_func(stat(fullpath_cmd, &buf));
		if ((buf.st_mode & S_IFMT) == S_IFDIR)
			exit(path_error(fullpath_cmd));
	}
	execve(fullpath_cmd, proc->command, get_environ(sh_var));
	exit(NO_CMD);
}

/*  execute single external command  */
static int	exec_single_external(t_expr *expr, t_proc *proc, t_sh_var *sh_var)
{
	int			wstatus;
	t_list		*proc_list;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	xsignal(SIGINT, SIG_IGN);
	expr->pid[0] = safe_func(fork());
	if (expr->pid[0] == 0)
		exec_single_child(expr, proc, 0, sh_var);
	safe_func(waitpid(expr->pid[0], &wstatus, WUNTRACED));
	last_proc_signal(wstatus);
	xsignal(SIGINT, sigint_handler);
	return (g_exit_status);
}

/*  execute single process  */
int	exec_single_proc(t_expr *expr, t_sh_var *sh_var)
{
	t_list		*proc_list;
	t_proc		*proc;
	int			res;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	res = set_io_and_commands(proc, sh_var);
	if (res)
	{
		g_exit_status = 1;
		return (g_exit_status);
	}
	if (proc->command[0] == NULL)
		return (0);
	if (is_builtin(proc->command[0]))
	{
		dup2_func(expr, proc, 0);
		close_func(expr, proc, 0);
		return (exec_builtin(proc, sh_var));
	}
	else
		return (exec_single_external(expr, proc, sh_var));
}
