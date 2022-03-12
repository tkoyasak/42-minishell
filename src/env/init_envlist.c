#include "shell_var.h"

t_list	*consume_new_env(t_list *cur, char *str)
{
	t_env	*env;

	env = ft_calloc(1, sizeof(t_env));
	env->key = ft_strndup(str, ft_strchr(str, '=') - str);
	env->val = ft_strdup(ft_strchr(str, '=') + 1);
	cur->next = ft_lstnew(env);
	return (cur->next);
}

t_list	*init_envlist(void)
{
	t_list		env_list;
	t_list		*cur;
	extern char	**environ;

	env_list.next = NULL;
	cur = &env_list;
	while (*environ)
	{
		cur = consume_new_env(cur, *environ);
		environ++;
	}
	return (env_list.next);
}

// int	main(void)
// {
// 	t_list	*env;

// 	env = init_env();
// 	while (env)
// 	{
// 		ft_putendl_fd(((t_env *)env->content)->key, STDOUT_FILENO);
// 		ft_putendl_fd(((t_env *)env->content)->val, STDOUT_FILENO);
// 		env = env->next;
// 	}
// 	return (0);
// }
