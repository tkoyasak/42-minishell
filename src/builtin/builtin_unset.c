#include "minishell.h"

int		builtin_unset(t_process *process, t_shell_var *shell_var)
{
	t_list	*itr;
	t_list	*next;
	t_list	*prev;
	char	*key;

	if (process->token_list->next == NULL)
		return (0);
	key = ((t_token *)(process->token_list->next->content))->str;
	prev = shell_var->env_list;
	itr = shell_var->env_list->next;
	while (itr)
	{
		next = itr->next;
		if (ft_strcmp(((t_env *)(itr->content))->key, key) == 0)
		{
			prev->next = next;
			free(((t_env *)(itr->content))->key);
			free(((t_env *)(itr->content))->val);
			free(itr->content);
			free(itr);
			break ;
		}
		prev = itr;
		itr = next;
	}
	return (0);
}
