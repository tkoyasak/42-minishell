#include "minishell.h"

char	*get_env_value(char *key, t_shell_var *shell_var)
{
	t_list	*itr;

	if (ft_strcmp(key, "?") == 0)
		return (ft_itoa(g_exit_status));
	itr = shell_var->env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)itr->content)->key, key) == 0)
		{
			if (((t_env *)(itr->content))->val)
				return (ft_xstrdup(((t_env *)(itr->content))->val));
			else
				return (NULL);
		}
		itr = itr->next;
	}
	return (NULL);
}

char	*get_env_value_str(char *key, t_shell_var *shell_var)
{
	char	*val;

	val = get_env_value(key, shell_var);
	if (val)
		return (val);
	else
		return (ft_xstrdup(""));
}