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

static t_list	*split_str_nonheredoc(char **str, bool *in_dquote, bool flag)
{
	t_list	*head;

	head = NULL;
	if (!*in_dquote && *(*str) == '\'')
	{
		ft_lstadd_back(&head, extract_word(str, true, flag, PD_SQUOTE));
		if (*(*str) == '\'')
			ft_lstadd_back(&head, create_zero_str(true, flag, PD_STRING));
		else
			ft_lstadd_back(&head, extract_word(str, true, flag, PD_STRING));
		ft_lstadd_back(&head, extract_word(str, true, flag, PD_SQUOTE));
	}
	else if (*(*str) == '\"')
	{
		*in_dquote ^= 1;
		ft_lstadd_back(&head, extract_word(str, false, true, PD_DQUOTE));
		if (*(*str) == '\"')
			ft_lstadd_back(&head, create_zero_str(false, true, PD_STRING));
	}
	else
		ft_lstadd_back(&head, extract_word(str, false, flag, PD_STRING));
	return (head);
}

t_list	*split_by_expd_kind(char *str, bool par_in_dquote, bool heredoc)
{
	t_list	*head;
	bool	in_dquote;
	bool	flag;

	if (!str)
		return (NULL);
	if (*str == '\0')
	{
		if (!par_in_dquote && !heredoc)
			return (NULL);
		else
			return (extract_word(&str, false, par_in_dquote, PD_STRING));
	}
	head = NULL;
	in_dquote = false;
	while (*str)
	{
		flag = par_in_dquote | in_dquote;
		if (heredoc)
			ft_lstadd_back(&head, extract_word(&str, false, false, PD_HEREDOC));
		else
			ft_lstadd_back(&head, split_str_nonheredoc(&str, &in_dquote, flag));
	}
	return (head);
}

/*  expand PD environment variable. quotes remain in heredoc  */
static void	expand_pd_env(t_list **head, t_list *itr, t_sh_var *sh_var, \
															bool heredoc)
{
	t_list	*delete_itr;
	t_expd	*expd;
	char	*env_str;

	expd = itr->content;
	env_str = expd->str;
	expd->str = get_env_value_str(expd->str + 1, sh_var);
	free(env_str);
	delete_itr = itr;
	itr = split_by_expd_kind(expd->str, expd->in_dquote, heredoc);
	ft_lstdelone(delete_itr, delete_expd);
	ft_lstadd_back(head, itr);
}

/*  expand environment variable
TK_STRING -> expd list       'aa'"b$HOME" ->  ' aa ' " b /Users/user " */
t_list	*expand_env(t_list *expd_list, t_sh_var *sh_var, bool heredoc)
{
	t_list	*head;
	t_list	*itr;
	t_list	*next;
	t_expd	*expd;

	head = NULL;
	itr = expd_list;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		expd = (t_expd *)(itr->content);
		if (expd->kind == PD_ENV)
			expand_pd_env(&head, itr, sh_var, heredoc);
		else
			ft_lstadd_back(&head, itr);
		itr = next;
	}
	return (head);
}
