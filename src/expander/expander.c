/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:17:10 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/24 11:17:11 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// expansion前のトークン１つを受け取って、展開して新しいトークン列を返す
// token->strをt_exp_strlist_typeで分割・分類する
// 環境変数の展開
static t_list	*get_expanded_token(t_list *token_list, t_sh_var *sh_var)
{
	t_token	*token;
	t_list	*expansion_list;
	t_list	*expanded_token_list;

	token = token_list->content;
	expansion_list = get_expansion_list(token->str, false, sh_var);
	expansion_list = get_filename_expansion(expansion_list);
	expansion_list = remove_quotes(expansion_list);
	expanded_token_list = convert_to_token_list(expansion_list);
	ft_lstdelone(token_list, delete_token);
	ft_lstclear(&expansion_list, delete_expansion);
	return (expanded_token_list);
}

static void	consume_token_to_expansion(t_list **itr, t_list **prev, t_list **next, t_sh_var *sh_var)
{
	*itr = get_expanded_token(*itr, sh_var);
	if (itr == NULL)
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

static void	consume_token_if_limiter(t_list **itr, t_list **prev, t_list **next, bool *is_limiter)
{
	t_token	*token;

	*is_limiter = false;
	token = (t_token *)(*itr)->content;
	if (token->kind == TK_IO && ft_strcmp(token->str, "<<") == 0)
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
			consume_token_to_expansion(&itr, &prev, &next, sh_var);
		else
			consume_token_if_limiter(&itr, &prev, &next, &is_limiter);
	}
	*token_list = head.next;
}

void	expansion(t_expr *expr, t_sh_var *sh_var)
{
	t_list	*itr;

	itr = expr->proc_list;
	while (itr)
	{
		handle_proc(&((t_proc *)(itr->content))->token_list, sh_var);
		g_exit_status = 0; // bashの挙動に合わせた
		itr = itr->next;
	}
}
