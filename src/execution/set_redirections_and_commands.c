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

void	remove_io_token(t_proc *proc)
{
	t_list	*itr;  // token_list
	t_list	*head;
	t_list	*tmp;

	head = NULL;
	itr = proc->token_list;
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
	proc->token_list = head;
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

int	set_io_input(t_proc *proc, t_list *itr, t_io_kind kind)
{
	proc->kind[0] = kind;
	proc->filename[0] = ((t_token *)(itr->content))->str;
	if (kind == IO_INPUT)
	{
		if (proc->fd[0])
			safe_func(close(proc->fd[0]));
		proc->fd[0] = open(proc->filename[0], R_OK);
		if (proc->fd[0] == -1)
		{
			open_error_handler(proc->filename[0]);
			g_exit_status = 1;
			return (-1);
		}
	}
	return (0);
}

int	set_io_output(t_proc *proc, t_list *itr, t_io_kind kind)
{
	if (proc->fd[1])
		safe_func(close(proc->fd[1]));
	proc->kind[1] = kind;
	proc->filename[1] = ((t_token *)(itr->content))->str;
	if (kind == IO_OUTPUT)
		proc->fd[1] = \
			open(proc->filename[1], O_CREAT | O_TRUNC | W_OK, 0644);
	else
		proc->fd[1] = \
			open(proc->filename[1], O_CREAT | O_APPEND | W_OK, 0644);
	if (proc->fd[1] == -1)
	{
		open_error_handler(proc->filename[1]);
		g_exit_status = 1;
		return (-1);
	}
	return (0);
}

// ls -al > file           ls -al  output_kind = IO_OUTPUT
// itr は token_list
void	set_io_params(t_proc *proc)
{
	t_list					*itr;
	t_io_kind		kind;

	itr = proc->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			kind = get_io_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == IO_INPUT || kind == IO_HEREDOC)
			{
				if (set_io_input(proc, itr, kind) == -1)
					return ;
			}
			else if (kind == IO_OUTPUT || kind == IO_APPEND)
			{
				if (set_io_output(proc, itr, kind) == -1)
					return ;
			}
		}
		itr = itr->next;
	}
}

void	set_command(t_proc *proc)
{
	t_list	*itr;
	int		cmd_idx;

	if (ft_lstsize(proc->token_list) == 0)
	{
		proc->command = ft_xcalloc(2, sizeof(char *));
		proc->command[0] = ft_xstrdup("");
		return ;
	}
	proc->command = \
			ft_xcalloc(ft_lstsize(proc->token_list) + 1, sizeof(char *));
	itr = proc->token_list;
	cmd_idx = 0;
	while (itr)
	{
		proc->command[cmd_idx] = ((t_token *)(itr->content))->str;
		itr = itr->next;
		cmd_idx++;
	}
}

void	set_io_and_commands(t_expr *expr)
{
	int			cmd_idx;
	t_list		*proc_list;
	t_proc	*proc;

	proc_list = expr->proc_list;
	cmd_idx = 0;
	while (cmd_idx < expr->proc_cnt)
	{
		proc = proc_list->content;
		set_io_params(proc);
		if (g_exit_status)
			return ;
		remove_io_token(proc);
		set_command(proc);
		proc_list = proc_list->next;
		cmd_idx++;
	}
}
