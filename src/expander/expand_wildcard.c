/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:17 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/26 11:04:21 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*get_matched_token_list(t_list *itr)
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
