/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_token_list.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:06 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 23:16:53 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*remove_quotes(t_list *src_list)
{
	t_list	*head;
	t_list	*itr;
	t_list	*next;
	t_expd	*expd;

	head = NULL;
	itr = src_list;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_SQUOTE || expd->kind == PD_DQUOTE)
			ft_lstdelone(itr, delete_expd);
		else
			ft_lstadd_back(&head, itr);
		itr = next;
	}
	return (head);
}

static size_t	token_str_len(t_list *src_list)
{
	size_t	len;
	t_list	*itr;
	t_expd	*expd;

	len = 0;
	itr = src_list;
	if (((t_expd *)(itr->content))->kind == PD_FILENAME)
		return (((t_expd *)(itr->content))->len);
	while (itr && ((t_expd *)(itr->content))->kind != PD_NAKED_SP && \
		((t_expd *)(itr->content))->kind != PD_FILENAME)
	{
		expd = (t_expd *)(itr->content);
		len += expd->len;
		itr = itr->next;
	}
	return (len);
}

static char	*consume_token_str_join(t_list **src_list, char *buf)
{
	t_list	*itr;
	size_t	len;
	t_expd	*expd;

	len = 0;
	itr = *src_list;
	if (((t_expd *)(itr->content))->kind == PD_FILENAME)
	{
		expd = (t_expd *)(itr->content);
		ft_strlcat(buf, expd->str, expd->len + 1);
		*src_list = (*src_list)->next;
		return (buf);
	}
	while (itr && ((t_expd *)(itr->content))->kind != PD_NAKED_SP && \
		((t_expd *)(itr->content))->kind != PD_FILENAME)
	{
		expd = (t_expd *)(itr->content);
		len += expd->len;
		ft_strlcat(buf, expd->str, len + 1);
		itr = itr->next;
	}
	*src_list = itr;
	return (buf);
}

static t_list	*consume_new_joined_token(t_list **itr)
{
	size_t	len;
	char	*str;
	t_token	*token;

	token = ft_xcalloc(1, sizeof(t_token));
	len = token_str_len(*itr);
	str = ft_xcalloc(len + 1, sizeof(char));
	token->str = consume_token_str_join(itr, str);
	token->kind = TK_STRING;
	return (ft_xlstnew(token));
}

/*  split by naked space and connect expd_list  */
t_list	*convert_to_token_list(t_list *expd_list)
{
	t_list	*head;
	t_list	*itr;
	t_expd	*expd;

	head = NULL;
	itr = expd_list;
	while (itr)
	{
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_NAKED_SP)
			itr = itr->next;
		else
			ft_lstadd_back(&head, consume_new_joined_token(&itr));
	}
	return (head);
}
