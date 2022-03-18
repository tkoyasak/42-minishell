#include "minishell.h"

int		builtin_pwd(t_process *process, t_shell_var *shell_var)
{
	(void)process;
	printf("%s\n", shell_var->pwd);
	return (0);
}
