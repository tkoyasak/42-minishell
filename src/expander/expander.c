/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:10 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/30 23:48:40 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*  expand one string and retrun token_list  */
t_list	*get_expanded_token_list(char *str, t_sh_var *sh_var)
{
	t_list	*expd_list;
	t_list	*expanded_token_list;

	expd_list = split_by_expd_kind(str, false, false);
	expd_list = expand_env(expd_list, sh_var, false);
	expd_list = expand_wildcard(expd_list);
	expd_list = remove_quotes(expd_list);
	expanded_token_list = convert_to_token_list(expd_list);
	ft_lstclear(&expd_list, delete_expd);
	return (expanded_token_list);
}

static void	consume_token_to_expand(t_list **itr, \
				t_list **prev, t_list **next, t_sh_var *sh_var)
{
	t_list	*delete_itr;

	delete_itr = *itr;
	*itr = get_expanded_token_list(((t_token *)(*itr)->content)->str, sh_var);
	ft_lstdelone(delete_itr, delete_token);
	if (*itr == NULL)
	{
		*itr = *next;
		(*prev)->next = *itr;
	}
	else
	{
		(*prev)->next = *itr;
		*itr = ft_lstlast(*itr);
		(*itr)->next = *next;
		*prev = *itr;
		*itr = (*itr)->next;
	}
}

static void	consume_token_if_io(t_list **itr, \
				t_list **prev, t_list **next, bool *is_limiter)
{
	t_token	*token;

	*is_limiter = false;
	token = (t_token *)(*itr)->content;
	if (token->kind == TK_IO)
		*is_limiter = true;
	(*itr)->next = *next;
	*prev = *itr;
	*itr = (*itr)->next;
}

static void	handle_proc(t_list **token_list, t_sh_var *sh_var)
{
	t_list	head;
	t_list	*itr;
	t_list	*next;
	t_list	*prev;
	bool	is_limiter;

	head.next = *token_list;
	itr = *token_list;
	prev = &head;
	is_limiter = false;
	while (itr)
	{
		next = itr->next;
		itr->next = NULL;
		if (!is_limiter && ((t_token *)(itr->content))->kind == TK_STRING)
			consume_token_to_expand(&itr, &prev, &next, sh_var);
		else
			consume_token_if_io(&itr, &prev, &next, &is_limiter);
	}
	*token_list = head.next;
}

void	expander(t_expr *expr, t_sh_var *sh_var)
{
	t_list	*itr;

	itr = expr->proc_list;
	while (itr)
	{
		handle_proc(&((t_proc *)(itr->content))->token_list, sh_var);
		if (itr->next)
			g_exit_status = 0;
		itr = itr->next;
	}
}
