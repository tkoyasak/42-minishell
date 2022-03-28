/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:17 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/28 10:58:44 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*get_matched_token_list(t_list *itr)
{
	t_list	*head;
	char	*str;
	char	*prefix;
	char	**slash_splitted_strs;
	int		i;

	str = ((t_expd *)(itr->content))->str;
	if (ft_strchr(str, '*') == NULL)
		return (itr);
	slash_splitted_strs = ft_xsplit(str, '/');
	if (*str == '/')
		prefix = ft_strdup("/");
	else
		prefix = ft_strdup("");
	head = matched_files(prefix, slash_splitted_strs);
	i = -1;
	while (slash_splitted_strs[++i])
		free(slash_splitted_strs[i]);
	free(slash_splitted_strs);
	free(prefix);
	if (head == NULL)
		return (itr);
	ft_lstdelone(itr, delete_expd);
	head = sorted_expd_list(head);
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
