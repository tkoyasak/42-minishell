/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_content2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 12:02:27 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 12:04:14 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	delete_node(void *arg)
{
	t_node	*node;

	if (!arg)
		return ;
	node = (t_node *)arg;
	ft_lstclear(&node->token_list, delete_token);
	delete_expr(node->expr);
	free(node);
	node = NULL;
}

void	delete_astree(t_node *node)
{
	if (!node)
		return ;
	if (node->kind == ND_SUBSHELL)
	{
		delete_node(node);
		return ;
	}
	if (node->lhs)
		delete_astree(node->lhs);
	if (node->rhs)
		delete_astree(node->rhs);
	delete_node(node);
}
