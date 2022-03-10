#include "builtin.h"

int		builtin_pwd(t_expression *expression, t_process *process, int cmd_idx)
{
	char	*buff;

	buff = getcwd(NULL, 0);
	write_in_process(expression, process, cmd_idx, buff);
	free(buff);
	return (0);
}
