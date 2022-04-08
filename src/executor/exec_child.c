/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:12:27 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 15:21:07 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	path_error(char *fullpath_cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(fullpath_cmd, STDERR_FILENO);
	ft_putendl_fd(": is a directory", STDERR_FILENO);
	return (NO_PERMISSION);
}

static void	exit_if_directory(char *fullpath_cmd)
{
	struct stat	buf;

	safe_func(stat(fullpath_cmd, &buf));
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
		exit(path_error(fullpath_cmd));
}

/*  execute child process  */
void	exec_child(t_expr *expr, t_proc *proc, int cmd_idx, t_sh_var *sh_var)
{
	char		*cmd;
	char		*fullpath_cmd;
	int			res;

	xsignal(SIGINT, SIG_DFL);
	xsignal(SIGQUIT, SIG_DFL);
	res = set_io_and_commands(proc, sh_var);
	dup2_func(expr, proc, cmd_idx);
	close_func(expr, proc, cmd_idx);
	if (res)
	{
		g_exit_status = 1;
		exit(1);
	}
	if (proc->command[0] == NULL)
		exit(0);
	cmd = proc->command[0];
	if (is_builtin(cmd))
		exit(exec_builtin(proc, sh_var));
	fullpath_cmd = get_fullpath_cmd(cmd, sh_var);
	if (fullpath_cmd)
		exit_if_directory(fullpath_cmd);
	execve(fullpath_cmd, proc->command, get_environ(sh_var));
	exit(NO_CMD);
}
