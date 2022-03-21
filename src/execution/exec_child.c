#include "minishell.h"

/*  execute child process  */
void	exec_child(t_expression *expression, t_process *process, const int cmd_idx, t_shell_var *shell_var)
{
	char		*cmd;
	char		*fullpath_cmd;
	struct stat	buf;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	cmd = ((t_token *)(process->token_list->content))->str;
	dup2_func(expression, process, cmd_idx);
	close_func(expression, process, cmd_idx);
	if (is_builtin(cmd))
		exit(exec_builtin(process, shell_var));
	fullpath_cmd = get_fullpath_cmd(cmd, shell_var);
	if (fullpath_cmd)
	{
		stat(fullpath_cmd, &buf);
		if ((buf.st_mode & S_IFMT) == S_IFDIR)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(fullpath_cmd, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			exit(126);
		}
	}
	execve(fullpath_cmd, process->command, get_environ(shell_var));
	exit(NOCMD);
}
