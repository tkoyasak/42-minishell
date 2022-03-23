#include "minishell.h"

t_io_kind	get_io_kind(char *redirect_str)
{
	const char			*redirect_strs[] = {"", "<", "<<", ">", ">>"};
	t_io_kind	kind;

	kind = IO_INPUT;
	while (kind < 5)
	{
		if (ft_strcmp(redirect_str, redirect_strs[kind]) == 0)
			return (kind);
		kind++;
	}
	return (IO_NONE);
}

void	remove_one_token(t_list **itr)
{
	t_list	*tmp;

	tmp = (*itr)->next;
	ft_lstdelone(*itr, delete_token);
	*itr = tmp;
}

void	remove_io_token(t_process *process)
{
	t_list	*itr;  // token_list
	t_list	*head;
	t_list	*tmp;

	head = NULL;
	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			remove_one_token(&itr);
			remove_one_token(&itr);
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

void	open_error_handler(char *filename)
{
	printf("errno:%d\n", errno);
	if (errno == EACCES)
	{
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
	}
	else if (errno == ENOENT)
	{
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	else if (errno == EISDIR)
	{
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
	}
}

int	set_io_input(t_process *process, t_list *itr, t_io_kind kind)
{
	process->kind[0] = kind;
	process->filename[0] = ((t_token *)(itr->content))->str;
	if (kind == IO_INPUT)
	{
		if (process->fd[0])
			safe_func(close(process->fd[0]));
		process->fd[0] = open(process->filename[0], R_OK);
		if (process->fd[0] == -1)
		{
			open_error_handler(process->filename[0]);
			g_exit_status = 1;
			return (-1);
		}
	}
	return (0);
}

int	set_io_output(t_process *process, t_list *itr, t_io_kind kind)
{
	if (process->fd[1])
		safe_func(close(process->fd[1]));
	process->kind[1] = kind;
	process->filename[1] = ((t_token *)(itr->content))->str;
	if (kind == IO_OUTPUT)
		process->fd[1] = \
			open(process->filename[1], O_CREAT | O_TRUNC | W_OK, 0644);
	else
		process->fd[1] = \
			open(process->filename[1], O_CREAT | O_APPEND | W_OK, 0644);
	if (process->fd[1] == -1)
	{
		open_error_handler(process->filename[1]);
		g_exit_status = 1;
		return (-1);
	}
	return (0);
}

// ls -al > file           ls -al  output_kind = IO_OUTPUT
// itr は token_list
void	set_io_params(t_process *process)
{
	t_list					*itr;
	t_io_kind		kind;

	itr = process->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			kind = get_io_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == IO_INPUT || kind == IO_HEREDOC)
			{
				if (set_io_input(process, itr, kind) == -1)
					return ;
			}
			else if (kind == IO_OUTPUT || kind == IO_APPEND)
			{
				if (set_io_output(process, itr, kind) == -1)
					return ;
			}
		}
		itr = itr->next;
	}
}

void	set_command(t_process *process)
{
	t_list	*itr;
	int		cmd_idx;

	if (ft_lstsize(process->token_list) == 0)
	{
		process->command = ft_xcalloc(2, sizeof(char *));
		process->command[0] = ft_xstrdup("");
		return ;
	}
	process->command = \
			ft_xcalloc(ft_lstsize(process->token_list) + 1, sizeof(char *));
	itr = process->token_list;
	cmd_idx = 0;
	while (itr)
	{
		process->command[cmd_idx] = ((t_token *)(itr->content))->str;
		itr = itr->next;
		cmd_idx++;
	}
}

void	set_io_and_commands(t_expr *expr)
{
	int			cmd_idx;
	t_list		*process_list;
	t_process	*process;

	process_list = expr->process_list;
	cmd_idx = 0;
	while (cmd_idx < expr->process_cnt)
	{
		process = process_list->content;
		set_io_params(process);
		if (g_exit_status)
			return ;
		remove_io_token(process);
		set_command(process);
		process_list = process_list->next;
		cmd_idx++;
	}
}
