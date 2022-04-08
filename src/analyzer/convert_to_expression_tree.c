/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_to_expression_tree.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 10:48:42 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/08 14:40:23 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static	t_list	*init_proc(t_list *token_list)
{
	t_proc	*proc;
	int 	i;
	
	proc = ft_xcalloc(1, sizeof(t_proc));
	proc->token_list = token_list;
	i = -1;
	while (++i < 2)
	{
		proc->fd[i] = FD_NONE;
		proc->fd_backup[i] = FD_NONE;
		proc->here_pipefd[i] = FD_NONE;
	}
	return (ft_xlstnew(proc));
}

static t_list	*convert_to_proc_list(t_node *tree)
{
	t_list	*proc_list;

	proc_list = NULL;
	if (tree->kind == ND_PIPE)
	{
		if (tree->lhs)
			ft_lstadd_back(&proc_list, convert_to_proc_list(tree->lhs));
		if (tree->rhs)
			ft_lstadd_back(&proc_list, convert_to_proc_list(tree->rhs));
	}
	else
	{
		proc_list = init_proc(tree->token_list);
		tree->token_list = NULL;
	}
	return (proc_list);
}

t_node	*convert_to_expr_tree(t_node *tree)
{
	t_expr	*expr;

	if (ND_SUBSHELL <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			convert_to_expr_tree(tree->lhs);
		if (tree->rhs)
			convert_to_expr_tree(tree->rhs);
	}
	else
	{
		expr = ft_xcalloc(1, sizeof(t_expr));
		expr->proc_list = convert_to_proc_list(tree);
		expr->proc_cnt = ft_lstsize(expr->proc_list);
		tree->expr = expr;
		tree->kind = ND_EXPR;
		tree->token_list = NULL;
	}
	return (tree);
}
