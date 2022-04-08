/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:13:45 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 14:32:01 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_pipe(t_expr *expr, const int cmd_idx)
{
	expr->pipefd[cmd_idx] = (int *)ft_xcalloc(2, sizeof(int));
	safe_func(pipe(expr->pipefd[cmd_idx]));
}

/* execute one process between pipes  */
static void	exec_one_proc(t_expr *expr, t_proc *proc, int cmd_idx, \
			t_sh_var *sh_var)
{
	if (cmd_idx < expr->proc_cnt - 1)
		create_pipe(expr, cmd_idx);
	xsignal(SIGINT, SIG_IGN);
	expr->pid[cmd_idx] = safe_func(fork());
	if (expr->pid[cmd_idx] == 0)
		exec_child(expr, proc, cmd_idx, sh_var);
	else if (cmd_idx)
	{
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (proc->fd[0] != FD_NONE)
		safe_func(close(proc->fd[0]));
	if (proc->fd[1] != FD_NONE)
		safe_func(close(proc->fd[1]));
}

static int	wait_all_procs(t_expr *expr, bool *caught_sigint)
{
	int		cmd_idx;
	int		wstatus;

	*caught_sigint = 0;
	cmd_idx = 0;
	while (cmd_idx < expr->proc_cnt)
	{
		safe_func(waitpid(expr->pid[cmd_idx], &wstatus, WUNTRACED));
		if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
			*caught_sigint = 1;
		cmd_idx++;
	}
	return (wstatus);
}

/*  execute processes between semicolons, double ampersand, and double pipe  */
int	exec_procs(t_expr *expr, t_sh_var *sh_var)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*proc_list;
	t_proc		*proc;
	bool		caught_sigint;

	cmd_idx = -1;
	proc_list = expr->proc_list;
	while (++cmd_idx < expr->proc_cnt)
	{
		proc = proc_list->content;
		exec_one_proc(expr, proc, cmd_idx, sh_var);
		proc_list = proc_list->next;
	}
	wstatus = wait_all_procs(expr, &caught_sigint);
	last_proc_signal(wstatus);
	if (!WIFSIGNALED(wstatus) && caught_sigint)
		ft_putchar_fd('\n', STDERR_FILENO);
	xsignal(SIGINT, sigint_handler);
	return (g_exit_status);
}
