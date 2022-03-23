#include "minishell.h"

int		builtin_pwd(t_proc *proc, t_sh_var *sh_var)
{
	(void)proc;
	printf("%s\n", sh_var->pwd);
	return (0);
}
