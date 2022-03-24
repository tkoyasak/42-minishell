/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_token.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:20 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/24 11:27:33 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*split_squote_str(char **str, bool in_dquote)
{
	t_list	*head;

	head = NULL;
	ft_lstadd_back(&head, extract_word(str, true, in_dquote, PD_SQUOTE));
	if (*(*str) == '\'')
		ft_lstadd_back(&head, create_zero_str(true, in_dquote, PD_STRING));
	else
		ft_lstadd_back(&head, extract_word(str, true, in_dquote, PD_STRING));
	ft_lstadd_back(&head, extract_word(str, true, in_dquote, PD_SQUOTE));
	return (head);
}

static t_list	*split_by_expd_kind(char *str, bool par_in_dquote, bool heredoc)
{
	t_list	*head;
	bool	in_dquote;
	bool	flag;

	if (!str)
		return (NULL);
	if (*str == '\0')
		return (extract_word(&str, false, par_in_dquote, PD_STRING));
	head = NULL;
	in_dquote = false;
	while (*str)
	{
		flag = par_in_dquote | in_dquote;
		if (heredoc)
			ft_lstadd_back(&head, extract_word(&str, false, flag, PD_HEREDOC));
		else if (!in_dquote && *str == '\'')
			ft_lstadd_back(&head, split_squote_str(&str, flag));
		else if (*str == '\"')
		{
			in_dquote ^= 1;
			ft_lstadd_back(&head, extract_word(&str, false, true, PD_DQUOTE));
		}
		else
			ft_lstadd_back(&head, extract_word(&str, false, flag, PD_STRING));
	}
	return (head);
}

t_list	*expand_token(char *str, bool par_in_dquote, bool heredoc, \
													t_sh_var *sh_var)
{
	t_list	head;
	t_list	*itr;
	t_list	*prev;
	t_list	*next;
	t_expd	*expd;

	head.next = split_by_expd_kind(str, par_in_dquote, heredoc);
	itr = head.next;
	prev = &head;
	while (itr)
	{
		next = itr->next;
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_ENV)
		{
			expd->str = get_env_value_str(expd->str + 1, sh_var);
			prev->next = expand_token(expd->str, expd->in_dquote, heredoc, sh_var);
			prev = ft_lstlast(head.next);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head.next);
}
