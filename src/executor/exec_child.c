/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:12:27 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 15:37:10 by tkoyasak         ###   ########.fr       */
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

/*  execute child process  */
void	exec_child(t_expr *expr, t_proc *proc, int cmd_idx, t_sh_var *sh_var)
{
	char		*cmd;
	char		*fullpath_cmd;
	struct stat	buf;

	xsignal(SIGINT, SIG_DFL);
	xsignal(SIGQUIT, SIG_DFL);
	g_exit_status = set_io_and_commands(proc, sh_var);
	dup2_func(expr, proc, cmd_idx);
	close_func(expr, proc, cmd_idx);
	if (g_exit_status)
		exit(1);
	if (proc->command == NULL)
		exit(0);
	cmd = proc->command[0];
	if (is_builtin(cmd))
		exit(exec_builtin(proc, sh_var));
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
