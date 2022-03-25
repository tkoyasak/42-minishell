/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:43:21 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/25 16:51:15 by jkosaka          ###   ########.fr       */
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

static int	set_io_filename(char **target_filename, char *str, t_sh_var *sh_var)
{
	t_list	*token_list;

	token_list = get_expanded_token_list(str, sh_var);
	if (ft_lstsize(token_list) != 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(str, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		ft_lstclear(&token_list, delete_token);
		return (1);
	}
	free(*target_filename);
	*target_filename = ft_xstrdup(((t_token *)(token_list->content))->str);
	ft_lstclear(&token_list, delete_token);
	return (0);
}

static int	set_input(t_proc *proc, t_list *itr, \
										t_io_kind kind, t_sh_var *sh_var)
{
	proc->kind[0] = kind;
	if (kind == IO_INPUT)
	{
		if (set_io_filename(&proc->filename[0], \
					((t_token *)(itr->content))->str, sh_var))
		{
			proc->kind[0] = IO_NONE;
			proc->kind[1] = IO_NONE;
			return (-1);
		}
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

int	set_io_output(t_proc *proc, t_list *itr, t_io_kind kind, t_sh_var *sh_var)
{
	if (proc->fd[1])
		safe_func(close(proc->fd[1]));
	proc->kind[1] = kind;
	if (set_io_filename(&proc->filename[1], \
						((t_token *)(itr->content))->str, sh_var))
	{
		proc->kind[0] = IO_NONE;
		proc->kind[1] = IO_NONE;
		return (-1);
	}
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
int	set_io_params(t_proc *proc, t_sh_var *sh_var)
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
				if (set_input(proc, itr, kind, sh_var) == -1)
					return (-1);
			}
			else if (kind == IO_OUTPUT || kind == IO_APPEND)
			{
				if (set_io_output(proc, itr, kind, sh_var) == -1)
					return (-1);
			}
		}
		itr = itr->next;
	}
	return (0);
}
