#include "execution.h"

// ls -al | cat | head -n2 ; ls ;  cat < file
int	execution(t_node *tree)
{
	int	error_status;
	t_list	*expression_list;

	expression_list = convert_to_expression_list(tree);
	while (expression_list)
	{
		error_status = evaluate_expression(((t_expression *)(expression_list->content)));
		expression_list = expression_list->next;
		// if (((t_expression *)(expression_list->content))->end_of_expression != )
		// 	;
	}
	return (error_status);
}

int	main(void)
{
	// t_node *tree = expansion("cat < infile | echo | 'hello'; $SHELL . ppp && echo hey! > outfile");
	// t_node *tree = expansion("ls -al | head -n2 | cat");
	t_node *tree = expansion("cat << EOS | cat > outfile");
	// t_list *expression_list = convert_to_expression_list(tree);
	int	error_status = execution(tree);

	printf("41:%d\n", error_status);
}

// int	main(void)
// {
// 	// t_node *tree = expansion("cat < infile | cat < infile2 | 'hello'; $SHELL && echo hey! > outfile");
// 	// t_node *tree = expansion("ls -a | cat");
// 	// t_node *tree = expansion("cat < infile | cat > outfile");
// 	// t_list *expression_list = convert_to_expression_list(tree);
// 	execution(tree);

// 	printf("==============\n");
// 	while (expression_list)
// 	{
// 		t_list *process_list = ((t_expression *)(expression_list->content))->process_list;
// 		static int cnt = 0; cnt++;
// 		printf("%d %d\n", cnt, ((t_expression *)(expression_list->content))->end_of_expression);
// 		while (process_list)
// 		{
// 			t_list *itr = ((t_process *)(process_list->content))->token_list;
// 			while (itr)
// 			{
// 				printf("%s ", ((t_token *)(itr->content))->str);
// 				itr = itr->next;
// 			}
// 			process_list = process_list->next;
// 			printf("\n");
// 		}
// 		expression_list = expression_list->next;
// 	}
// }
