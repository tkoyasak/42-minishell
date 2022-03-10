#include "builtin.h"

void	write_in_process(t_expression *expression, t_process *process, int cmd_idx, char *str)
{
	if (process->kind[1] == NONE)
	{
		// redirection なし
		if (cmd_idx < expression->process_cnt - 1)
			write(expression->pipefd[cmd_idx][1], str, ft_strlen(str));
		else
			write(STDOUT_FILENO, str, ft_strlen(str));
	}
	else
	{
		// redirection あり OUTPUTとAPPEND共通
		write(process->fd[1], str, ft_strlen(str));
	}
}
