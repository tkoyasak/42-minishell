#include "minishell.h"

int	exec_single_external(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	int			wstatus;
	t_list		*process_list;

	process_list = expression->process_list;
	process = process_list->content;
	expression->pid[0] = safe_func(fork());
	if (expression->pid[0] == 0)
		exec_child(expression, process, 0, shell_var);
	process_list = process_list->next;
	safe_func(waitpid(expression->pid[0], &wstatus, WUNTRACED));
	return (WEXITSTATUS(wstatus));
}

int	exec_single_process(t_expression *expression, t_shell_var *shell_var)
{
	t_list		*process_list;
	t_process	*process;

	set_redirections_and_commands(expression, shell_var);
	if (g_exit_status)
		return (g_exit_status);
	process_list = expression->process_list;
	process = process_list->content;
	dup2_func(expression, process, 0);
	close_func(expression, process, 0);
	if (is_builtin(process->command[0]))
		return (exec_builtin(process, shell_var));
	else
		return (exec_single_external(expression, process, shell_var));
}
