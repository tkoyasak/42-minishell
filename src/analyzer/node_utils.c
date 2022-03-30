/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 10:48:30 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/30 11:01:14 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parser_error(t_list **itr, char *str, bool *is_valid)
{
	if (*is_valid)
	{
		*is_valid = false;
		ft_putstr_fd("minishell: syntax error near unexpected token `", \
			STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd("\'", STDERR_FILENO);
		ft_lstclear(itr, delete_token);
	}
}

t_node	*node_new(t_node_kind kind, t_node *lhs, t_node *rhs)
{
	t_node	*node;

	node = ft_xcalloc(1, sizeof(t_node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return (node);
}

/*  compare string with operator, and if match consume iterator  */
bool	consume_node_kind(t_list **itr, char *op, bool *is_valid)
{
	t_list	*delete_itr;

	if (!*is_valid)
		return (false);
	if (*itr && ft_strcmp(((t_token *)((*itr)->content))->str, op) == 0)
	{
		delete_itr = *itr;
		*itr = (*itr)->next;
		ft_lstdelone(delete_itr, delete_token);
		return (true);
	}
	return (false);
}

/*  consume itr while token kind string or io  */
static	bool	consume_one_proc(t_list **itr)
{
	if (((t_token *)((*itr)->content))->kind != TK_IO && \
		((t_token *)((*itr)->content))->kind != TK_STRING)
		return (false);
	while ((*itr)->next != NULL && \
		(((t_token *)((*itr)->next->content))->kind == TK_IO || \
		((t_token *)((*itr)->next->content))->kind == TK_STRING))
	{
		if (((t_token *)(*itr)->content)->kind == TK_IO && \
				((t_token *)((*itr)->next->content))->kind != TK_STRING)
			return (false);
		*itr = (*itr)->next;
	}
	if (*itr && ((t_token *)((*itr)->content))->kind != TK_STRING)
		return (false);
	return (true);
}

t_node	*create_proc_node(t_list **itr, bool *is_valid)
{
	t_node	*node;
	t_list	*tail;

	if (!*is_valid || !*itr)
		return (NULL);
	node = ft_xcalloc(1, sizeof(t_node));
	node->kind = ND_PROC;
	node->token_list = *itr;
	if (consume_one_proc(itr))
	{
		tail = *itr;
		*itr = tail->next;
		tail->next = NULL;
		return (node);
	}
	else
	{
		parser_error(&(node->token_list), \
				((t_token *)(*itr)->content)->str, is_valid);
		delete_node(node);
		return (NULL);
	}
}
