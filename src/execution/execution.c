#include "minishell.h"

// pipefd[0] = {input fd, output fd}
void	init_expression(t_expression *expression)
{
	int	pipe_cnt;

	expression->process_cnt = ft_lstsize(expression->process_list);  // 3
	pipe_cnt = expression->process_cnt - 1;
	expression->pipefd = (int **)ft_calloc(pipe_cnt, sizeof(int *)); // prepare_pipe pipeは2つ用意
	expression->pid = (pid_t *)ft_calloc(expression->process_cnt, sizeof(pid_t));
}

// ls -al |  cat  |  head -n2  ここがpipex
int	evaluate_expression(t_expression *expression, t_shell_var *shell_var)
{
	int	error_status;

	init_expression(expression);
	if (expression->process_cnt == 1)
		error_status = exec_single_process(expression, shell_var);
	else
		error_status = exec_processes(expression, shell_var);
	return (error_status);
}

// ls -al | cat | head -n2 ; ls ;  cat < file
int	execution(t_node *tree, t_shell_var *shell_var)
{
	int				error_status;
	t_list			*expression_list;
	t_expression	*expression;

	expression_list = convert_to_expression_list(tree);;
	while (expression_list)
	{
		expression = expression_list->content;
		error_status = evaluate_expression(expression, shell_var);
		if (((t_expression *)(expression_list->content))->end_of_expression == ND_DAND \
				&& error_status != 0)
			break ;
		if (((t_expression *)(expression_list->content))->end_of_expression == ND_DPIPE \
				&& error_status == 0)
			break ;
		expression_list = expression_list->next;
	}
	return (error_status);
}

// int	main(void)
// {
// 	t_shell_var	*shell_var;

// 	shell_var = NULL;
// 	// t_node *tree = expansion("cat < infile | echo | 'hello'; $SHELL . ppp && echo hey! > outfile");
// 	// t_node *tree = expansion("ls -al | head -n4 | cat");
// 	t_node *tree = expansion("cd ..");
// 	// t_node *tree = expansion("cd docs");
// 	// t_node *tree = expansion("export | head -n6 | cat");
// 	// t_list *expression_list = convert_to_expression_list(tree);
// 	int	error_status = execution(tree, shell_var);

// 	printf("41:%d\n", error_status);
// }

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
