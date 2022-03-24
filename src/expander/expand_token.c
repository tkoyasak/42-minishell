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

static size_t	get_word_len(char *str, bool in_squote, bool in_dquote)
{
	char	*head;

	head = str;
	if ((in_squote && *str == '\'') || (!in_squote && *str == '\"'))
		return (1);
	if (in_squote)
		return (ft_strchr(str, '\'') - str);
	if (*str == '$')
	{
		str++;
		if (*str == '?')
			return (2);
		while (*str && (ft_isalnum(*str) || *str == '_'))
			str++;
	}
	else if (in_dquote)
		while (*str && *str != '$' && *str != '\"')
			str++;
	else if (ft_isspace(*str))
		while (*str && ft_isspace(*str))
			str++;
	else
		while (*str && !ft_strchr("\'\"$ ", *str))
			str++;
	return (str - head);
}

static t_list	*extract_word(char **str, bool in_squote, bool in_dquote, t_expd_kind kind)
{
	t_expd	*expd;

	expd = ft_xcalloc(1, sizeof(t_expd));
	expd->str = ft_xsubstr(*str, 0, get_word_len(*str, in_squote, in_dquote));
	expd->len = ft_strlen(expd->str);
	*str += expd->len;
	expd->in_squote = in_squote;
	expd->in_dquote = in_dquote;
	expd->kind = kind;
	if (!in_squote && expd->str && expd->str[0] == '$' && \
		(ft_isalnum(expd->str[1]) || expd->str[1] == '_' || expd->str[1] == '?'))
		expd->kind = PD_ENV;
	if (!in_squote && !in_dquote && expd->str && expd->str[0] == ' ')
		expd->kind = PD_NAKED_SP;
	return (ft_xlstnew(expd));
}

static t_list	*create_zero_str(bool in_squote, bool in_dquote, t_expd_kind kind)
{
	t_expd	*expd;

	expd = ft_xcalloc(1, sizeof(t_expd));
	expd->str = ft_xstrdup("");
	expd->len = 0;
	expd->in_squote = in_squote;
	expd->in_dquote = in_dquote;
	expd->kind = kind;
	return (ft_xlstnew(expd));
}

static t_list	*split_by_expd_kid(char *str, bool par_in_dquote)
{
	t_list	*head;
	bool	in_dquote;
	bool	flag;

	head = NULL;
	in_dquote = false;
	while (*str)
	{
		flag = par_in_dquote | in_dquote;
		if (!in_dquote && *str == '\'')
		{
			ft_lstadd_back(&head, extract_word(&str, true, flag, PD_SQUOTE));
			if (str[0] == '\'')
			{
				ft_lstadd_back(&head, create_zero_str(true, flag, PD_STRING));
			}
			else
				ft_lstadd_back(&head, extract_word(&str, true, flag, PD_STRING));
			ft_lstadd_back(&head, extract_word(&str, true, flag, PD_SQUOTE));
		}
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

static t_list	*split_token(char *str, bool par_in_dquote)
{
	if (!str)
		return (NULL);
	if (*str == '\0')
		return (extract_word(&str, false, par_in_dquote, PD_STRING));
	return (split_by_expd_kid(str, par_in_dquote));
}

t_list	*expand_token(char *str, bool par_in_dquote, t_sh_var *sh_var)
{
	t_list	head;
	t_list	*itr;
	t_list	*prev;
	t_list	*next;
	t_expd	*expd;

	head.next = split_token(str, par_in_dquote);
	itr = head.next;
	prev = &head;
	while (itr)
	{
		next = itr->next;
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_ENV)
		{
			expd->str = get_env_value_str(expd->str + 1, sh_var);
			prev->next = expand_token(expd->str, expd->in_dquote, sh_var);
			prev = ft_lstlast(head.next);
			prev->next = next;
		}
		else
			prev = itr;
		itr = next;
	}
	return (head.next);
}
