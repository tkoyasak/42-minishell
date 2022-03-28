/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sorted_expd_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/28 08:26:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/28 11:09:08 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	swap_expd(t_expd **a, t_expd **b)
{
	t_expd	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static t_expd	**bubble_expd_sort(t_expd **expd_list, int size)
{
	int	i;
	int	j;

	i = 0;
	j = size - 1;
	while (i < j)
	{
		while (i < j)
		{
			if (ft_strcmp(expd_list[i]->str, expd_list[j]->str) > 0)
				swap_expd(&(expd_list[i]), &(expd_list[i+1]));
			i++;
		}
		i = 0;
		j--;
	}
	return (expd_list);
}

/*  return sorted expander list  */
t_list	*sorted_expd_list(t_list *lst)
{
	t_expd	**expd_list;
	t_list	*itr;
	int		size;
	int		idx;

	size = ft_lstsize(lst);
	expd_list = (t_expd **)malloc(sizeof(t_expd *) * size);
	itr = lst;
	idx = 0;
	while (itr)
	{
		expd_list[idx] = itr->content;
		itr = itr->next;
		idx++;
	}
	expd_list = bubble_expd_sort(expd_list, size);
	itr = lst;
	idx = 0;
	while (itr)
	{
		itr->content = expd_list[idx];
		itr = itr->next;
		idx++;
	}
	free(expd_list);
	return (lst);
}
