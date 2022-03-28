/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:17 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/28 15:56:44 by jkosaka          ###   ########.fr       */
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
	int	target;
	int	end;

	target = 0;
	end = size - 1;
	while (target < end)
	{
		while (target < end)
		{
			if (ft_strcmp(expd_list[target]->str, expd_list[target + 1]->str) > 0)
				swap_expd(&(expd_list[target]), &(expd_list[target + 1]));
			target++;
		}
		target = 0;
		end--;
	}
	return (expd_list);
}

static void	sort_expd_list(t_list *lst)
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
}

static t_list	*get_matched_token_list(t_list *itr)
{
	t_list	*head;
	char	*str;
	char	*prefix;
	char	**slash_splitted_strs;
	int		target;

	str = ((t_expd *)(itr->content))->str;
	if (ft_strchr(str, '*') == NULL)
		return (itr);
	slash_splitted_strs = ft_xsplit(str, '/');
	if (*str == '/')
		prefix = ft_strdup("/");
	else
		prefix = ft_strdup("");
	head = matched_files(prefix, slash_splitted_strs);
	target = -1;
	while (slash_splitted_strs[++target])
		free(slash_splitted_strs[target]);
	free(slash_splitted_strs);
	free(prefix);
	if (head == NULL)
		return (itr);
	ft_lstdelone(itr, delete_expd);
	sort_expd_list(head);
	return (head);
}

/*  if naked PD_STRING, expand wildcard  */
t_list	*expand_wildcard(t_list *expd_list)
{
	t_list	head;
	t_list	*itr;
	t_list	*next;
	t_list	*prev;

	head.next = NULL;
	prev = &head;
	itr = expd_list;
	while (itr)
	{
		next = itr->next;
		if (((t_expd *)(itr->content))->kind == PD_STRING && \
			((t_expd *)(itr->content))->in_dquote == false && \
			((t_expd *)(itr->content))->in_squote == false)
			itr = get_matched_token_list(itr);
		prev->next = itr;
		while (itr->next != NULL && itr->next != next)
			itr = itr->next;
		itr->next = next;
		prev = itr;
		itr = itr->next;
	}
	return (head.next);
}
