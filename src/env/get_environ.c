#include "env.h"

char	**get_environ(void)
{
	char	**environ;
	t_list	*env_list;
	t_list	*itr;
	size_t	i;
	char	*key;

	i = 0;
	environ = ft_calloc(ft_lstsize(env_list) + 1, sizeof(char *));
	itr = env_list;
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