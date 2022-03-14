#include "minishell.h"

int	builtin_echo(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	t_list			*itr;
	char			*str;
	bool			has_endl;

	has_endl = true;
	itr = process->token_list->next;
	str = ((t_token *)(itr->content))->str;
	if (ft_strcmp(str, "-n") == 0)
	{
		has_endl = false;
		itr = itr->next;
	}
	while (itr)
	{
		str = ((t_token *)(itr->content))->str;
		itr = itr->next;
		printf("%s", str);
		if (itr)
			printf(" ");
	}
	if (has_endl)
		printf("\n");
	return (0);
}
