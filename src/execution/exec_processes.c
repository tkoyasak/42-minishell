#include "minishell.h"

static void	create_pipe(t_expr *expr, const int cmd_idx)
{
	expr->pipefd[cmd_idx] = (int *)ft_xcalloc(2, sizeof(int));
	safe_func(pipe(expr->pipefd[cmd_idx]));
}

static int	wait_all_processes(t_expr *expr)
{
	int	cmd_idx;
	int	wstatus;

	cmd_idx = 0;
	while (cmd_idx < expr->process_cnt)
	{
		safe_func(waitpid(expr->pid[cmd_idx], &wstatus, WUNTRACED));
		cmd_idx++;
	}
	return (wstatus);
}

/* execute one process between pipes  */
static void	exec_one_process(t_expr *expr, t_process *process, int cmd_idx, t_shell_var *shell_var)
{
	if (cmd_idx < expr->process_cnt - 1)
		create_pipe(expr, cmd_idx);
	signal(SIGINT, SIG_IGN);
	expr->pid[cmd_idx] = safe_func(fork());
	if (expr->pid[cmd_idx] == 0)
		exec_child(expr, process, cmd_idx, shell_var);
	else if (cmd_idx)
	{
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (process->fd[0])
		safe_func(close(process->fd[0]));
	if (process->fd[1])
		safe_func(close(process->fd[1]));
}

/*  execute processes between semicolons, double ampersand, and double pipe  */
int	exec_processes(t_expr *expr, t_shell_var *shell_var)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	cmd_idx = 0;
	process_list = expr->process_list;
	set_io_and_commands(expr);
	if (g_exit_status)
		return (g_exit_status);
	while (cmd_idx < expr->process_cnt)
	{
		process = process_list->content;
		exec_one_process(expr, process, cmd_idx, shell_var);
		process_list = process_list->next;
		cmd_idx++;
	}
	wstatus = wait_all_processes(expr);
	signal(SIGINT, sigint_handler);
	return (WEXITSTATUS(wstatus));
}
