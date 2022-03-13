#include "builtin.h"
#include "shell_var.h"

int		builtin_env(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	t_list	*env_list = init_envlist();
	t_list	*itr;

	itr = env_list;
	while (itr)
	{
		printf("%s=%s\n", ((t_env *)(itr->content))->key, ((t_env *)(itr->content))->val);
		itr = itr->next;
	}
	// printf("called builtin_env\n");
	return (0);
}
