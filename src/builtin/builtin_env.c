#include "builtin.h"
#include "shell_var.h"

int		builtin_env(t_expression *expression, t_process *process, int cmd_idx)
{
	t_list	*env_list = init_envlist();
	t_list	*itr;

	itr = env_list;
	while (itr)
	{
		write_in_process(expression, process, cmd_idx, ((t_env *)(itr->content))->key);
		write_in_process(expression, process, cmd_idx, "=");
		write_in_process(expression, process, cmd_idx, ((t_env *)(itr->content))->val);
		write_in_process(expression, process, cmd_idx, "\n");
		itr = itr->next;
	}
	printf("called builtin_env\n");
	return (0);
}
