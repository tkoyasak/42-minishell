#include "minishell.h"

int		builtin_pwd(t_proc *proc, t_shell_var *shell_var)
{
	(void)proc;
	printf("%s\n", shell_var->pwd);
	return (0);
}
