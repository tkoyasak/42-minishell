#include "builtin.h"

int		builtin_pwd(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	char	*buff;

	buff = getcwd(NULL, 0);
	printf("%s\n", buff);
	free(buff);
	return (0);
}
