#include "builtin.h"

int		builtin_pwd(t_expression *expression, t_process *process, int cmd_idx)
{
	char	*buff;

	buff = getcwd(NULL, 0);
	if (process->kind[1] == NONE)
	{
		// redirection なし
		if (cmd_idx < expression->process_cnt - 1)
			write(expression->pipefd[cmd_idx][1], buff, ft_strlen(buff));
		else
			write(STDOUT_FILENO, buff, ft_strlen(buff));
	}
	else
	{
		// redirection あり OUTPUTとAPPEND共通
		write(process->fd[1], buff, ft_strlen(buff));
	}
	free(buff);
	return (0);
}
