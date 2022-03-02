#include "execution.h"

t_redirection_kind	get_redirection_kind(char *redirect_str)
{
	const char	*redirect_strs[] = {"", "<", "<<", ">", ">>"};
	t_expansion_kind	kind;

	kind = INPUT;
	while (kind < 5)
	{
		if (ft_strcmp(redirect_str, redirect_strs[kind]) == 0)
			return (kind);
		kind++;
	}
	return (NONE);
}

// ls -al > file           ls -al  output_kind = OUTPUT
void	set_redirection_params(t_process *process)
{
	t_list					*itr;  // token_list
	t_redirection_kind		kind;

	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_REDIRECT)
		{
			kind = get_redirection_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == INPUT || kind == HEREDOC)
			{
				process->input_kind = kind;
				process->input_filename = ((t_token *)(itr->content))->str;
			}
			else if (kind == OUTPUT || kind == APPEND)
			{
				process->output_kind = kind;
				process->output_filename = ((t_token *)(itr->content))->str;
			}
		}
		itr = itr->next;
	}
}

// pipex + redirect + builtin
int	exec_process(t_process *process)
{
	set_redirection_params(process);
	return (127);
}

int	evaluate_expression(t_list *process_list)
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
		error_status = evaluate_expression(((t_expression *)(expression_list->content))->process_list);
		expression_list = expression_list->next;
		// if (((t_expression *)(expression_list->content))->end_of_expression != )
		// 	;
	}
	return (error_status);
}

int	main(void)
{
	t_node *tree = expansion("cat < infile | echo | 'hello'; $SHELL . ppp && echo hey! > outfile");
	t_list *expression_list = convert_to_expression_list(tree);
	int	error_status = execute(expression_list);

	printf("41:%d\n", error_status);
}
