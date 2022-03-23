#include "minishell.h"

int		builtin_env(t_proc *proc, t_sh_var *sh_var)
{
	t_list	*itr;

	(void)proc;
	itr = sh_var->env_list->next;
	while (itr)
	{
		printf("%s=%s\n", ((t_env *)(itr->content))->key, ((t_env *)(itr->content))->val);
		itr = itr->next;
	}
	return (0);
}
