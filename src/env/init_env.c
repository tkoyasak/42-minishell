#include "env.h"

t_list	*consume_new_item(t_list *cur, char *env)
{
	t_item	*item;
	size_t	key_len;

	item = ft_calloc(1, sizeof(t_item));
	key_len = ft_strchr(env, '=') - env;
	item->key = ft_substr(env, 0, key_len);
	item->val = ft_substr(env, key_len + 1, ft_strlen(env) - key_len);
	cur->next = ft_lstnew(item);
	return (cur->next);
}

t_list	*init_env(void)
{
	t_list		item_list;
	t_list		*cur;
	extern char	**environ;

	item_list.next = NULL;
	cur = &item_list;
	while (*environ)
	{
		cur = consume_new_item(cur, *environ);
		environ++;
	}
	return (item_list.next);
}

// int	main(void)
// {
// 	t_list	*env;

// 	env = init_env();
// 	while (env)
// 	{
// 		ft_putendl_fd(((t_item *)env->content)->key, STDOUT_FILENO);
// 		ft_putendl_fd(((t_item *)env->content)->val, STDOUT_FILENO);
// 		env = env->next;
// 	}
// 	return (0);
// }
