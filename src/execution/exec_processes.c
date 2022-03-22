#include "minishell.h"

static void	create_pipe(t_expression *expression, const int cmd_idx)
{
	expression->pipefd[cmd_idx] = (int *)ft_calloc(2, sizeof(int));
	if (!expression->pipefd[cmd_idx])
		exit(EXIT_FAILURE);
	if (pipe(expression->pipefd[cmd_idx]) < 0)
		exit(EXIT_FAILURE);
}

static int	wait_all_processes(t_expression *expression)
{
	int	cmd_idx;
	int	wstatus;

	cmd_idx = 0;
	while (cmd_idx < expression->process_cnt)
	{
		waitpid(expression->pid[cmd_idx], &wstatus, WUNTRACED);
		cmd_idx++;
	}
	return (wstatus);
}

/* execute one process between pipes  */
static void	exec_one_process(t_expression *expression, t_process *process, int cmd_idx, t_shell_var *shell_var)
{
	if (cmd_idx < expression->process_cnt - 1)
		create_pipe(expression, cmd_idx);
	signal(SIGINT, SIG_IGN);
	expression->pid[cmd_idx] = safe_func(fork());
	if (expression->pid[cmd_idx] == 0)
		exec_child(expression, process, cmd_idx, shell_var);
	else if (cmd_idx)
	{
		safe_func(close(expression->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expression->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (process->fd[0])
		safe_func(close(process->fd[0]));
	if (process->fd[1])
		safe_func(close(process->fd[1]));
}

/*  execute processes between semicolons, double ampersand, and double pipe  */
int	exec_processes(t_expression *expression, t_shell_var *shell_var)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	cmd_idx = 0;
	process_list = expression->process_list;
	set_redirections_and_commands(expression, shell_var);
	if (g_exit_status)
		return (g_exit_status);
	while (cmd_idx < expression->process_cnt)
	{
		process = process_list->content;
		exec_one_process(expression, process, cmd_idx, shell_var);
		process_list = process_list->next;
		cmd_idx++;
	}
	wstatus = wait_all_processes(expression);
	signal(SIGINT, sigint_handler);
	return (WEXITSTATUS(wstatus));
}
