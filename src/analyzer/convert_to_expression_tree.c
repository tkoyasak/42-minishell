#include "minishell.h"

t_list	*convert_to_process_list(t_node *tree)
{
	t_list		*process_list;
	t_process	*process;

	process_list = NULL;
	if (tree->kind == ND_PIPE)
	{
		if (tree->lhs)
			ft_lstadd_back(&process_list, convert_to_process_list(tree->lhs));
		if (tree->rhs)
			ft_lstadd_back(&process_list, convert_to_process_list(tree->rhs));
	}
	else
	{
		process = ft_calloc(1, sizeof(t_process));
		process->token_list = tree->token_list;
		process_list = ft_lstnew(process);
	}
	return (process_list);
}

t_node	*convert_to_expression_tree(t_node *tree)
{
	t_expression	*expression;

	if (ND_SUBSHELL <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
			convert_to_expression_tree(tree->lhs);
		if (tree->rhs)
			convert_to_expression_tree(tree->rhs);
	}
	else
	{
		expression = ft_calloc(1, sizeof(t_expression));
		expression->process_list = convert_to_process_list(tree);
		expression->process_cnt = ft_lstsize(expression->process_list);
		tree->expression = expression;
		tree->kind = ND_EXPRESSION;
		tree->token_list = NULL;
	}
	return (tree);
}
