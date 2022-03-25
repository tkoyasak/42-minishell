/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:54:11 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 12:03:37 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_expanded_len_heredoc(t_list *exp_list)
{
	size_t	len;

	len = 0;
	while (exp_list)
	{
		len += ((t_expd *)(exp_list->content))->len;
		exp_list = exp_list->next;
	}
	return (len);
}

char	*remove_quote_heredoc(char *limiter, bool *in_quote)
{
	char	*dst;
	size_t	src_idx;
	size_t	dst_idx;

	dst = ft_xcalloc(ft_strlen(limiter) + 1, sizeof(char));
	src_idx = 0;
	dst_idx = 0;
	while (limiter[src_idx])
	{
		if (ft_strchr(QUOTE_CHAR, limiter[src_idx]))
		{
			*in_quote = true;
			src_idx++;
			continue ;
		}
		dst[dst_idx++] = limiter[src_idx++];
	}
	return (dst);
}

/*  expand env word in here document like aa$PATH  */
char	*expander_heredoc(char *str, t_sh_var *sh_var)
{
	char	*dst;
	char	*cur;
	size_t	len;
	t_list	*expd_list;
	t_list	*head;

	expd_list = split_by_expd_kind(str, false, true);
	expd_list = expand_env(expd_list, sh_var, true);
	len = get_expanded_len_heredoc(expd_list);
	dst = (char *)ft_xmalloc(sizeof(char) * (len + 1));
	dst[0] = '\0';
	head = expd_list;
	while (expd_list)
	{
		cur = ((t_expd *)(expd_list->content))->str;
		ft_strlcat(dst, cur, ft_strlen(dst) + ft_strlen(cur) + 1);
		expd_list = expd_list->next;
	}
	ft_lstclear(&head, delete_expd);
	return (dst);
}
