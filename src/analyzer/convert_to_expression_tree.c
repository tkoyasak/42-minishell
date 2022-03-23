#include "minishell.h"

t_list	*convert_to_proc_list(t_node *tree)
{
	t_list		*proc_list;
	t_proc	*proc;

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
		proc = ft_calloc(1, sizeof(t_proc));
		proc->token_list = tree->token_list;
		proc_list = ft_lstnew(proc);
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
		expr = ft_calloc(1, sizeof(t_expr));
		expr->proc_list = convert_to_proc_list(tree);
		expr->proc_cnt = ft_lstsize(expr->proc_list);
		tree->expr = expr;
		tree->kind = ND_EXPR;
		tree->token_list = NULL;
	}
	return (tree);
}
