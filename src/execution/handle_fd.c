#include "minishell.h"

void	dup2_func(t_expr *expr, t_proc *proc, const int cmd_idx)
{
	if (proc->kind[0] == IO_NONE && cmd_idx > 0)
		safe_func(dup2(expr->pipefd[cmd_idx - 1][PIPEIN], STDIN_FILENO));
	else if (proc->kind[0] == IO_HEREDOC)
		safe_func(dup2(proc->here_fd, STDIN_FILENO));
	else if (proc->kind[0] != IO_NONE)
		safe_func(dup2(proc->fd[0], STDIN_FILENO));
	if (proc->kind[1] == IO_NONE && cmd_idx < expr->proc_cnt - 1)
		safe_func(dup2(expr->pipefd[cmd_idx][PIPEOUT], STDOUT_FILENO));
	else if (proc->kind[1] != IO_NONE)
		safe_func(dup2(proc->fd[1], STDOUT_FILENO));
}

void	close_func(t_expr *expr, t_proc *proc, const int cmd_idx)
{
	if (proc->kind[0] == IO_HEREDOC)
		safe_func(close(proc->here_fd));
	if (cmd_idx > 0)
	{
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expr->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (cmd_idx < expr->proc_cnt - 1)
	{
		safe_func(close(expr->pipefd[cmd_idx][PIPEIN]));
		safe_func(close(expr->pipefd[cmd_idx][PIPEOUT]));
	}
}
