#include "minishell.h"

void	dup2_func(t_expression *expression, t_process *process, const int cmd_idx)
{
	if (process->kind[0] == NONE && cmd_idx > 0) // cmd_idx > 0 かつ redirectionがない
		safe_func(dup2(expression->pipefd[cmd_idx - 1][PIPEIN], STDIN_FILENO));
	else if (process->kind[0] == HEREDOC)
		safe_func(dup2(process->here_fd, STDIN_FILENO));
	else if (process->kind[0] != NONE)
		safe_func(dup2(process->fd[0], STDIN_FILENO));

	if (process->kind[1] == NONE && cmd_idx < expression->process_cnt - 1)
		safe_func(dup2(expression->pipefd[cmd_idx][PIPEOUT], STDOUT_FILENO));
	else if (process->kind[1] != NONE)
		safe_func(dup2(process->fd[1], STDOUT_FILENO));
}

void	close_func(t_expression *expression, t_process *process, const int cmd_idx)
{
	if (process->kind[0] == HEREDOC)
		safe_func(close(process->here_fd));
	if (cmd_idx > 0)
	{
		safe_func(close(expression->pipefd[cmd_idx - 1][PIPEIN]));
		safe_func(close(expression->pipefd[cmd_idx - 1][PIPEOUT]));
	}
	if (cmd_idx < expression->process_cnt - 1)
	{
		safe_func(close(expression->pipefd[cmd_idx][PIPEIN]));
		safe_func(close(expression->pipefd[cmd_idx][PIPEOUT]));
	}
}
