#include "minishell.h"

char	**get_environ(t_shell_var *shell_var)
{
	char	**environ;
	t_list	*itr;
	size_t	i;
	char	*key;

	i = 0;
	environ = ft_calloc(ft_lstsize(shell_var->env_list), sizeof(char *));
	itr = shell_var->env_list->next;
	while (itr)
	{
		key = ft_strjoin(((t_env *)itr->content)->key, "=");
		environ[i] = ft_strjoin(key, ((t_env *)itr->content)->val);
		free(key);
		itr = itr->next;
		i++;
	}
	return (environ);
}
