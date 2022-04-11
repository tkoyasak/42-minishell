/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:12:27 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/10 21:50:26 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_if_directory(char *cmd_path)
{
	struct stat	buf;

	if (!cmd_path)
		return ;
	if (stat(cmd_path, &buf) == -1)
		return ;
	if (ft_strchr(cmd_path, '/') == NULL)
		return ;
	if ((buf.st_mode & S_IFMT) == S_IFDIR)
	{
		print_error_msg(cmd_path, "is a directory");
		exit(NO_PERMISSION);
	}
}

bool	is_executable(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == -1)
		return (false);
	if ((path_stat.st_mode & S_IRUSR) == 0) // 読み込み権限がない場合
		return (false);
	if ((path_stat.st_mode & S_IXUSR) == 0) // 実行権限がない場合
		return (false);
	return (true);
}

static void		execve_error_handler(char *path)
{
	// printf("errno:%d\n", errno);
	if (errno == ENOENT)
	{
		if (ft_strchr(path, '/'))
			print_error_msg(path, strerror(errno));
		else
			print_error_msg(path, "command not found");
		exit(NO_CMD);
	}
	if (!is_executable(path))
		errno = EACCES;
	print_error_msg(path, strerror(errno));
	exit(NO_PERMISSION);
}

/*  execute child process  */
void	exec_child(t_expr *expr, t_proc *proc, int cmd_idx, t_sh_var *sh_var)
{
	char		*cmd;
	char		*cmd_path;
	int			res;
	char		**environ;

	res = set_io_params(proc, sh_var);
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
	cmd_path = get_cmd_path(cmd, sh_var);
	exit_if_directory(cmd_path);
	environ = get_environ(sh_var);
	execve(cmd_path, proc->command, environ);
	ft_split_free(environ);
	execve_error_handler(cmd);
	exit(NO_CMD);
}
