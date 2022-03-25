/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:13:45 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 17:39:53 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	create_pipe(t_expr *expr, const int cmd_idx)
{
	expr->pipefd[cmd_idx] = (int *)ft_xcalloc(2, sizeof(int));
	safe_func(pipe(expr->pipefd[cmd_idx]));
}

static int	wait_all_procs(t_expr *expr)
{
	int	cmd_idx;
	int	wstatus;

	cmd_idx = 0;
	while (cmd_idx < expr->proc_cnt)
	{
		safe_func(waitpid(expr->pid[cmd_idx], &wstatus, WUNTRACED));
		cmd_idx++;
	}
	return (wstatus);
}

/* execute one process between pipes  */
static void	exec_one_proc(t_expr *expr, t_proc *proc, int cmd_idx, \
			t_sh_var *sh_var)
{
	if (cmd_idx < expr->proc_cnt - 1)
		create_pipe(expr, cmd_idx);
	signal(SIGINT, SIG_IGN);
	expr->pid[cmd_idx] = safe_func(fork());
	if (expr->pid[cmd_idx] == 0)
		exec_child(expr, proc, cmd_idx, sh_var);
	else if (cmd_idx)
	{
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (proc->fd[0])
		safe_func(close(proc->fd[0]));
	if (proc->fd[1])
		safe_func(close(proc->fd[1]));
}

/*  execute processes between semicolons, double ampersand, and double pipe  */
int	exec_procs(t_expr *expr, t_sh_var *sh_var)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*proc_list;
	t_proc		*proc;

	cmd_idx = -1;
	proc_list = expr->proc_list;
	while (++cmd_idx < expr->proc_cnt)
	{
		proc = proc_list->content;
		exec_one_proc(expr, proc, cmd_idx, sh_var);
		proc_list = proc_list->next;
	}
	wstatus = wait_all_procs(expr);
	signal(SIGINT, sigint_handler);
	return (WEXITSTATUS(wstatus));
}
