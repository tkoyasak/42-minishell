/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:17 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/24 17:18:41 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_matched_token_list(t_list *expd_list)
{
	t_expd	*expd;
	t_list	*head;
	char	*str;
	char	**slash_splitted_strs;

	expd = expd_list->content;
	str = expd->str;
	if (ft_strchr(str, '*') == NULL)
		return (expd_list);
	slash_splitted_strs = ft_xsplit(str, '/');
	if (*str == '/')
		head = matched_files("/", slash_splitted_strs);
	else
		head = matched_files("", slash_splitted_strs);
	if (head == NULL)
		return (expd_list);
	return (head);
}

/*   t_expd_kindがSTRINGでかつin_squoteとin_dquoteがfalseで、
*を含むものを展開   */
t_list	*expand_wildcard(t_list *expansion_list)
{
	t_list	head;
	t_list	*itr;
	t_list	*next;
	t_list	*prev;

	head.next = NULL;
	prev = &head;
	itr = expansion_list;
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
