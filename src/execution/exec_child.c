#include "minishell.h"

/*  execute child proc  */
void	exec_child(t_expr *expr, t_proc *proc, const int cmd_idx, t_shell_var *shell_var)
{
	char		*cmd;
	char		*fullpath_cmd;
	struct stat	buf;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd = ((t_token *)(proc->token_list->content))->str;
	dup2_func(expr, proc, cmd_idx);
	close_func(expr, proc, cmd_idx);
	if (is_builtin(cmd))
		exit(exec_builtin(proc, shell_var));
	fullpath_cmd = get_fullpath_cmd(cmd, shell_var);
	if (fullpath_cmd)
	{
		safe_func(stat(fullpath_cmd, &buf));
		if ((buf.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(fullpath_cmd, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			exit(NO_PERMISSION);
		}
	}
	execve(fullpath_cmd, proc->command, get_environ(shell_var));
	exit(NO_CMD);
}
