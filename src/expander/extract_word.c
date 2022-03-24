/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_word.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:58:22 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/24 19:25:00 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	expd_str_len(char *str, bool in_squote, bool in_dquote)
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
		if (*str == '?' || ft_isdigit(*str))
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

static size_t	heredoc_str_len(char *str)
{
	char	*head;

	head = str;
	if (*str == '$')
	{
		str++;
		if (*str == '?' || ft_isdigit(*str))
			return (2);
		while (*str && (ft_isalnum(*str) || *str == '_'))
			str++;
	}
	else
	{
		while (*str && *str != '$')
			str++;
	}
	return (str - head);
}

t_list	*extract_word(char **str, bool in_squote, bool in_dquote, \
													t_expd_kind kind)
{
	t_expd	*expd;

	expd = ft_xcalloc(1, sizeof(t_expd));
	if (kind == PD_HEREDOC)
		expd->len = heredoc_str_len(*str);
	else
		expd->len = expd_str_len(*str, in_squote, in_dquote);
	expd->str = ft_strndup(*str, expd->len);
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

t_list	*create_zero_str(bool in_squote, bool in_dquote, t_expd_kind kind)
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
