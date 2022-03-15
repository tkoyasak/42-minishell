#include "minishell.h"

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
void	set_redirection_params(t_process *process, t_shell_var *shell_var)
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
				process->kind[0] = kind;
				process->filename[0] = ((t_token *)(itr->content))->str;
				if (kind == INPUT)
					process->fd[0] = open(process->filename[0], R_OK); //調べる
				else
					set_heredoc(process, process->filename[0], shell_var);
			}
			else if (kind == OUTPUT || kind == APPEND)
			{
				process->kind[1] = kind;
				process->filename[1] = ((t_token *)(itr->content))->str;
				if (kind == OUTPUT)
					process->fd[1] = open(process->filename[1], O_CREAT | O_TRUNC | W_OK, 0644);
				else
					process->fd[1] = open(process->filename[1], O_CREAT | O_APPEND | W_OK, 0644);
			}
		}
		itr = itr->next;
	}
}

void	set_command(t_process *process)
{
	t_list	*itr;
	int		cmd_idx;

	remove_redirection_token(process);
	process->command = ft_calloc(ft_lstsize(process->token_list) + 1, sizeof(char *));
	itr = process->token_list;
	cmd_idx = 0;
	while (itr)
	{
		process->command[cmd_idx] = ((t_token *)(itr->content))->str;
		itr = itr->next;
		cmd_idx++;
	}
}

static void	create_pipe(t_expression *expression, const int cmd_idx)
{
	expression->pipefd[cmd_idx] = (int *)ft_calloc(2, sizeof(int));
	if (!expression->pipefd[cmd_idx])
		exit(EXIT_FAILURE);
	if (pipe(expression->pipefd[cmd_idx]) < 0)
		exit(EXIT_FAILURE);
}

void	dup2_func(t_expression *expression, t_process *process, const int cmd_idx)
{
	if (process->kind[0] == NONE && cmd_idx > 0) // cmd_idx > 0 かつ redirectionがない
		dup2(expression->pipefd[cmd_idx - 1][PIPEIN], STDIN_FILENO);
	else if (process->kind[0] == HEREDOC)
	{
		process->here_pipefd = ft_calloc(2, sizeof(int));
		if (!process->here_pipefd)
			exit(EXIT_FAILURE);
		if (pipe(process->here_pipefd) < 0)
			exit(EXIT_FAILURE);
		// heredocの$?を展開
		write(process->here_pipefd[1], process->heredoc, ft_strlen(process->heredoc));
		dup2(process->here_pipefd[0], STDIN_FILENO);
	}
	else if (process->kind[0] != NONE)
		dup2(process->fd[0], STDIN_FILENO);

	if (process->kind[1] == NONE && cmd_idx < expression->process_cnt - 1)
		dup2(expression->pipefd[cmd_idx][PIPEOUT], STDOUT_FILENO);
	else if (process->kind[1] != NONE)
		dup2(process->fd[1], STDOUT_FILENO);
}

void	close_func(t_expression *expression, t_process *process, const int cmd_idx)
{
	if (process->kind[0] == HEREDOC)
	{
		close(process->here_pipefd[0]);
		close(process->here_pipefd[1]);
	}
	if (cmd_idx > 0)
	{
		close(expression->pipefd[cmd_idx - 1][PIPEIN]);
		close(expression->pipefd[cmd_idx - 1][PIPEOUT]);
	}
	if (cmd_idx < expression->process_cnt - 1)
	{
		close(expression->pipefd[cmd_idx][PIPEIN]);
		close(expression->pipefd[cmd_idx][PIPEOUT]);
	}
}

/*  execute child process  */
void	exec_child(t_expression *expression, t_process *process, const int cmd_idx, t_shell_var *shell_var)
{
	char	*cmd;
	char	*fullpath_cmd;

	cmd = ((t_token *)(process->token_list->content))->str;
	dup2_func(expression, process, cmd_idx);
	close_func(expression, process, cmd_idx);
	if (is_builtin(cmd))
		exit(exec_builtin(expression, process, shell_var));
	fullpath_cmd = get_fullpath_cmd(cmd, shell_var);
	if (cmd_idx < expression->process_cnt - 1)
		g_exit_status = 0;
	execve(fullpath_cmd, process->command, get_environ(shell_var));
	exit(NOCMD);
}

static int	wait_all_processes(t_expression *expression)
{
	int	cmd_idx;
	int	wstatus;

	cmd_idx = 0;
	while (cmd_idx < expression->process_cnt)
	{
		waitpid(expression->pid[cmd_idx], &wstatus, WUNTRACED);
		cmd_idx++;
	}
	return (wstatus);
}

void	set_redirections_and_commands(t_expression *expression, t_shell_var *shell_var)
{
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	process_list = expression->process_list;
	cmd_idx = 0;
	while (cmd_idx < expression->process_cnt)
	{
		process = process_list->content;
		set_redirection_params(process, shell_var);
		set_command(process);
		process_list = process_list->next;
		cmd_idx++;
	}
}

// redirect + builtin
int	exec_processes(t_expression *expression, t_shell_var *shell_var)
{
	int			wstatus;
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	cmd_idx = 0;
	process_list = expression->process_list;
	set_redirections_and_commands(expression, shell_var);
	while (cmd_idx < expression->process_cnt)
	{
		process = process_list->content;
		if (cmd_idx < expression->process_cnt - 1)
			create_pipe(expression, cmd_idx);
		expression->pid[cmd_idx] = fork();
		if (expression->pid[cmd_idx] == 0)
			exec_child(expression, process, cmd_idx, shell_var);
		else if (cmd_idx)
		{
			close(expression->pipefd[cmd_idx - 1][PIPEIN]);
			close(expression->pipefd[cmd_idx - 1][PIPEOUT]);
			// close(process->fd[0]);
			// close(process->fd[1]);
		}
		process_list = process_list->next;
		cmd_idx++;
	}
	wstatus = wait_all_processes(expression);
	return (WEXITSTATUS(wstatus));
}
