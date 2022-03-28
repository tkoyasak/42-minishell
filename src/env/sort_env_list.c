/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_env_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 08:26:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/28 15:58:45 by jkosaka          ###   ########.fr       */
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
	int	target;
	int	end;

	target = 0;
	end = size - 1;
	while (end > 0)
	{
		while (target < end)
		{
			if (ft_strcmp(env_list[target]->key, env_list[target+1]->key) > 0)
				swap_env(&(env_list[target]), &(env_list[target+1]));
			target++;
		}
		target = 0;
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
	env_list = (t_env **)malloc(sizeof(t_env *) * size);
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
