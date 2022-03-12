#include "builtin.h"

int	builtin_echo(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	t_list			*itr;
	char			*str;

	itr = process->token_list->next;
	while (itr)
	{
		str = ((t_token *)(itr->content))->str;
		itr = itr->next;
		if (itr)
			str = ft_strjoin(str, " ");
		printf("%s\n", str);
	}
	return (0);
}
