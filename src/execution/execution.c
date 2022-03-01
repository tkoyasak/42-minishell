#include "execution.h"

// ls -al > file           ls -al  output_kind = OUTPUT
void	set_redirection_params(t_process *process)
{
	t_token	*new_token;

	process->token = new_token;

}

// pipex + redirect + builtin 
int	exec_process(t_process *process)
{
	
	
	return (127);
}

int	exec_expression(t_list *process_list)
{
	int	error_status;

	while (process_list)
	{
		error_status = exec_process((t_process *)(process_list->content));
		process_list = process_list->next;
	}
	return (error_status);
}

int	execute(t_list *expression_list)
{
	int	error_status;

	while (expression_list)
	{
		error_status = exec_process(((t_expression *)(expression_list->content))->process);
		expression_list = expression_list->next;
		// if (((t_expression *)(expression_list->content))->end_of_expression != )
		// 	;
	}
	printf("%d\n", error_status);
	return (error_status);
}

int	main(void)
{
	t_node *tree = expansion("ls -al | echo | 'hello'; $SHELL . ppp && echo hey!");
	t_list *expression_list = convert_to_expression_list(tree);
	int	error_status = execute(expression_list);

	printf("41:%d\n", error_status);
}
