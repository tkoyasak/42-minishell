#include "minishell.h"

/*  execute single external command  */
int	exec_single_external(t_expr *expr, t_proc *proc, t_shell_var *shell_var)
{
	int			wstatus;
	t_list		*proc_list;

	proc_list = expr->proc_list;
	proc = proc_list->content;
	signal(SIGINT, SIG_IGN);
	expr->pid[0] = safe_func(fork());
	if (expr->pid[0] == 0)
		exec_child(expr, proc, 0, shell_var);
	safe_func(waitpid(expr->pid[0], &wstatus, WUNTRACED));
	if (WIFSIGNALED(wstatus))
	{
		if (WTERMSIG(wstatus) == SIGQUIT)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		else if (WTERMSIG(wstatus) == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		g_exit_status = WTERMSIG(wstatus) + 128;
	}
	else
		g_exit_status = WEXITSTATUS(wstatus);
	signal(SIGINT, sigint_handler);
	return (g_exit_status);
}

/*  execute single proc  */
int	exec_single_proc(t_expr *expr, t_shell_var *shell_var)
{
	t_list		*proc_list;
	t_proc	*proc;

	set_io_and_commands(expr);
	if (g_exit_status)
		return (g_exit_status);
	proc_list = expr->proc_list;
	proc = proc_list->content;
	if (is_builtin(proc->command[0]))
	{
		dup2_func(expr, proc, 0);
		close_func(expr, proc, 0);
		return (exec_builtin(proc, shell_var));
	}
	else
		return (exec_single_external(expr, proc, shell_var));
}
