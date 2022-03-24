/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:54:11 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/24 23:57:26 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*extract_word_heredoc(char **str, bool in_squote, \
		bool in_dquote, t_expd_kind kind)
{
	t_list	*new;
	t_expd	*expd;

	expd = ft_xcalloc(1, sizeof(t_expd));
	expd->str = \
		ft_xsubstr(*str, 0, get_word_len_heredoc(*str, in_squote, in_dquote));
	expd->len = ft_strlen(expd->str);
	*str += expd->len;
	expd->in_squote = in_squote;
	expd->in_dquote = in_dquote;
	expd->kind = kind;
	if (expd->str && expd->str[0] == '$' && ft_isalnum(expd->str[1]) && !in_squote)
		expd->kind = PD_ENV;
	new = ft_xlstnew(expd);
	return (new);
}

void	connect_expansion_list_heredoc(t_expd *expd, t_list *head, \
		t_list *prev, t_sh_var *sh_var)
{
	if (prev == NULL)
		head = get_expansion_list_heredoc(expd->str, expd->in_dquote, sh_var);
	else
		prev->next = \
				get_expansion_list_heredoc(expd->str, expd->in_dquote, sh_var);
	prev = ft_lstlast(head);
}

t_list	*get_expansion_list_heredoc(char *str, bool par_in_dquote, \
		t_sh_var *sh_var)
{
	t_list		*head;
	t_list		*itr;
	t_list		*prev;
	t_list		*next;
	t_expd		*expd;

	prev = NULL;
	head = split_str_heredoc(str, par_in_dquote);
	itr = head;
	while (itr)
	{
		next = itr->next;
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_ENV)
		{
			expd->str = get_env_value_str(expd->str + 1, sh_var);
			connect_expansion_list_heredoc(expd, head, prev, sh_var);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head);
}

/*  expand env word in here document like aa$PATH  */
char	*expansion_heredoc(char *str, t_sh_var *sh_var)
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
