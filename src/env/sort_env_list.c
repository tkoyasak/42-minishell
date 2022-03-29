/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 08:26:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 00:16:11 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_env(t_env **a, t_env **b)
{
	t_env	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static t_env	**bubble_env_sort(t_env **env_list, int size)
{
	int	idx;
	int	end;

	idx = 0;
	end = size - 1;
	while (end > 0)
	{
		while (idx < end)
		{
			if (ft_strcmp(env_list[idx]->key, env_list[idx + 1]->key) > 0)
				swap_env(&(env_list[idx]), &(env_list[idx + 1]));
			idx++;
		}
		idx = 0;
		end--;
	}
	return (env_list);
}

void	sort_env_list(t_list *lst)
{
	t_env	**env_list;
	t_list	*itr;
	int		size;
	int		idx;

	size = ft_lstsize(lst);
	env_list = (t_env **)ft_xmalloc(sizeof(t_env *) * size);
	itr = lst;
	idx = 0;
	while (itr)
	{
		env_list[idx] = itr->content;
		itr = itr->next;
		idx++;
	}
	env_list = bubble_env_sort(env_list, size);
	itr = lst;
	idx = 0;
	while (itr)
	{
		itr->content = env_list[idx];
		itr = itr->next;
		idx++;
	}
	free(env_list);
}
