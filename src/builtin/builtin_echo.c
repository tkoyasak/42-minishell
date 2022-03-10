#include "builtin.h"

int	builtin_echo(t_expression *expression, t_process *process, int cmd_idx)
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
		write_in_process(expression, process, cmd_idx, str);
	}
	return (0);
}
