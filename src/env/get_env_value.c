#include "minishell.h"

char	*get_env_value(char *key, t_shell_var *shell_var)
{
	t_list	*itr;

	itr = shell_var->env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)itr->content)->key, key) == 0)
			return (ft_strdup(((t_env *)(itr->content))->val));
		itr = itr->next;
	}
	if (itr == NULL)
		return (ft_strdup(""));
}
