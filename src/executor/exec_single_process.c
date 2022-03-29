/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_single_process.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:16:41 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 00:17:17 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  execute single child process  */
static void	exec_single_child(t_expr *expr, t_proc *proc, int cmd_idx, \
															t_sh_var *sh_var)
{
	char		*cmd;
	char		*fullpath_cmd;
	struct stat	buf;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd = ((t_token *)(proc->token_list->content))->str;
	dup2_func(expr, proc, cmd_idx);
	close_func(expr, proc, cmd_idx);
	if (g_exit_status)
		exit(1);
	fullpath_cmd = get_fullpath_cmd(cmd, sh_var);
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
int	exec_single_external(t_expr *expr, t_proc *proc, t_sh_var *sh_var)
{
	int			wstatus;
	t_list		*proc_list;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	signal(SIGINT, SIG_IGN);
	expr->pid[0] = safe_func(fork());
	if (expr->pid[0] == 0)
		exec_single_child(expr, proc, 0, sh_var);
	safe_func(waitpid(expr->pid[0], &wstatus, WUNTRACED));
	last_proc_signal(wstatus);
	signal(SIGINT, sigint_handler);
	return (g_exit_status);
}

/*  execute single process  */
int	exec_single_proc(t_expr *expr, t_sh_var *sh_var)
{
	t_list		*proc_list;
	t_proc		*proc;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	g_exit_status = set_io_and_commands(proc, sh_var);
	if (g_exit_status)
		return (g_exit_status);
	if (proc->command == NULL)
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
