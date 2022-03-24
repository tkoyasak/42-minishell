/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:43:21 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/24 11:52:34 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static int	set_input(t_proc *proc, t_list *itr, t_io_kind kind)
{
	proc->kind[0] = kind;
	proc->filename[0] = ft_xstrdup(((t_token *)(itr->content))->str);
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
	proc->filename[1] = ft_xstrdup(((t_token *)(itr->content))->str);
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

/*  set input/output parameters  */
void	set_io_params(t_proc *proc)
{
	t_list		*itr;
	t_io_kind	kind;

	itr = proc->token_list;
	while (itr)
	{
		if (((t_token *)(itr->content))->kind == TK_IO)
		{
			kind = get_io_kind(((t_token *)(itr->content))->str);
			itr = itr->next;
			if (kind == IO_INPUT || kind == IO_HEREDOC)
			{
				if (set_input(proc, itr, kind) == -1)
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
