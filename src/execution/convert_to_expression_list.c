#include "execution.h"

t_list *convert_to_process_list(t_node *tree)
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
		process = malloc(sizeof(t_process));
		process->token = tree->token;
		process_list = ft_lstnew(process);
	}
	return (process_list);
}

t_list *convert_to_expression_list(t_node *tree)
{
	t_list			*expression_list;
	t_expression 	*expression;

	expression_list = NULL;
	if (ND_SEMICOLON <= tree->kind && tree->kind <= ND_DPIPE)
	{
		if (tree->lhs)
		{
			ft_lstadd_back(&expression_list, convert_to_expression_list(tree->lhs));
			((t_expression *)(ft_lstlast(expression_list)->content))->end_of_expression = tree->kind;
		}
		if (tree->rhs)
		{
			expression = malloc(sizeof(t_expression));
			expression->process = convert_to_process_list(tree->rhs);
			ft_lstadd_back(&expression_list, ft_lstnew(expression));
			((t_expression *)(ft_lstlast(expression_list)->content))->end_of_expression = ND_SEMICOLON;
		}
	}
	else
	{
		expression = malloc(sizeof(t_expression));
		expression->process = convert_to_process_list(tree);
		ft_lstadd_back(&expression_list, ft_lstnew(expression));
	}
	return (expression_list);
}

// int	main(void)
// {
// 	t_node *tree = expansion("ls -al | echo | 'hello'; $SHELL . ppp && echo hey!");
// 	t_list *expression_list = convert_to_expression_list(tree);

// 	while (expression_list)
// 	{
// 		t_list *process_list = ((t_expression *)(expression_list->content))->process;
// 		static int cnt = 0; cnt++;
// 		printf("%d %d\n", cnt, ((t_expression *)(expression_list->content))->end_of_expression);
// 		while (process_list)
// 		{
// 			t_token *tok = ((t_process *)(process_list->content))->token;
// 			while (tok)
// 			{
// 				printf("%s ", tok->str);
// 				tok = tok->next;
// 			}
// 			process_list = process_list->next;
// 			printf("\n");
// 		}
// 		expression_list = expression_list->next;
// 	}
// }