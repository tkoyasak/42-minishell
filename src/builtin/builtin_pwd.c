#include "minishell.h"

int		builtin_pwd(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	printf("%s\n", shell_var->pwd);
	return (0);
}
