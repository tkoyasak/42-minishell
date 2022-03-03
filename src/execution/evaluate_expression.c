#include "execution.h"

t_redirection_kind	get_redirection_kind(char *redirect_str)
{
	const char	*redirect_strs[] = {"", "<", "<<", ">", ">>"};
	t_redirection_kind	kind;

	kind = INPUT;
	while (kind < 5)
	{
		if (ft_strcmp(redirect_str, redirect_strs[kind]) == 0)
			return (kind);
		kind++;
	}
	return (NONE);
}

void	remove_redirection_token(t_process *process)
{
	t_list					*itr;  // token_list
	t_redirection_kind		kind;
	t_list					*head;
	t_list					*tmp;

	head = NULL;
	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_REDIRECT)
		{
			tmp = itr->next;
			// ft_lstdelone(&itr, del_token);
			itr = tmp;
			tmp = itr->next;
			// ft_lstdelone(&itr, del_token);
			itr = tmp;
		}
		else
		{
			tmp = itr;
			itr = itr->next;
			tmp->next = NULL;
			ft_lstadd_back(&head, tmp);
		}
	}
	process->token_list = head;
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
				if (kind == INPUT)
					process->input_fd = open(process->input_filename, R_OK); //調べる
				else
					;// heredocの処理
			}
			else if (kind == OUTPUT || kind == APPEND)
			{
				// file 作成
				process->output_kind = kind;
				process->output_filename = ((t_token *)(itr->content))->str;
				if (kind == OUTPUT)
					process->output_fd = open(process->output_filename, O_CREAT | O_TRUNC | W_OK, 0644);
				else
					process->output_fd = open(process->output_filename, O_CREAT | O_APPEND | W_OK, 0644);
			}
		}
		itr = itr->next;
	}
	remove_redirection_token(process);
}


static void	create_pipe(t_expression *expression, const int cmd_idx)
{
	expression->pipefd[cmd_idx] = (int *)ft_calloc(2, sizeof(int));
	if (!expression->pipefd[cmd_idx])
		exit(EXIT_FAILURE);
	if (pipe(expression->pipefd[cmd_idx]) < 0)
		exit(EXIT_FAILURE);
}

// redirect + builtin
int	exec_processes(t_expression *expression)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	cmd_idx = 0;
	process_list = expression->process_list;
	while (cmd_idx < expression->process_cnt)
	{
		process = process_list->content;
		set_redirection_params(process);
		if (cmd_idx < expression->process_cnt - 1)
			create_pipe(expression, cmd_idx);
		// fork exec_child
		process_list = process_list->next;
		cmd_idx++;
	}
	return (127);
}

// pipefd[0] = {input fd, output fd}
void	init_expression(t_expression *expression)
{
	int	pipe_cnt;

	expression->process_cnt = ft_lstsize(expression->process_list);  // 3
	pipe_cnt = expression->process_cnt - 1;
	expression->pipefd = (int **)ft_calloc(pipe_cnt, sizeof(int *)); // prepare_pipe pipeは2つ用意
	printf("63: pipe_cnt %d\n", pipe_cnt);
	expression->pid = (pid_t *)ft_calloc(expression->process_cnt, sizeof(pid_t)); 
}

// ls -al |  cat  |  head -n2  ここがpipex
int	evaluate_expression(t_expression *expression)
{
	int	error_status;

	init_expression(expression);
	error_status = exec_processes(expression);
	return (error_status);
}
