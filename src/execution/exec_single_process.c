#include "minishell.h"

/*  execute single external command  */
int	exec_single_external(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	int			wstatus;
	t_list		*process_list;

	process_list = expression->process_list;
	process = process_list->content;
	signal(SIGINT, SIG_IGN);
	expression->pid[0] = safe_func(fork());
	if (expression->pid[0] == 0)
		exec_child(expression, process, 0, shell_var);
	safe_func(waitpid(expression->pid[0], &wstatus, WUNTRACED));
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
	return (WEXITSTATUS(wstatus));
}

/*  execute single process  */
int	exec_single_process(t_expression *expression, t_shell_var *shell_var)
{
	t_list		*process_list;
	t_process	*process;

	set_redirections_and_commands(expression, shell_var);
	if (g_exit_status)
		return (g_exit_status);
	process_list = expression->process_list;
	process = process_list->content;
	if (is_builtin(process->command[0]))
	{
		dup2_func(expression, process, 0);
		close_func(expression, process, 0);
		return (exec_builtin(process, shell_var));
	}
	else
		return (exec_single_external(expression, process, shell_var));
}
