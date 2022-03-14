#include "minishell.h"

int		builtin_unset(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	t_list	*env_list = init_envlist();
	t_list	head;
	t_list	*itr;
	t_list	*next;
	t_list	*prev;
	char	*key;

	key = ((t_token *)(process->token_list->next->content))->str;
	head.next = env_list;
	prev = &head;
	itr = env_list;
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
