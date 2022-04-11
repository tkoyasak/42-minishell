/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_params.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:43:21 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/11 12:24:17 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	open_error_handler(char *filename)
{
	print_error_msg(filename, strerror(errno));
	errno = 0;
}

static int	set_io_filename(char **target_filename, char *str, t_sh_var *sh_var)
{
	t_list	*token_list;

	token_list = get_expanded_token_list(str, sh_var);
	if (ft_lstsize(token_list) != 1)
	{
		print_error_msg(str, "ambiguous redirect");
		ft_lstclear(&token_list, delete_token);
		return (1);
	}
	free(*target_filename);
	*target_filename = ft_xstrdup(((t_token *)(token_list->content))->str);
	ft_lstclear(&token_list, delete_token);
	return (0);
}

static int	open_file(char *file, t_io_kind kind)
{
	if (kind == IO_INPUT)
		return (open(file, O_RDONLY));
	else if (kind == IO_OUTPUT)
		return (open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else
		return (open(file, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static int	open_io_file(t_proc *proc, t_list *itr, \
										t_io_kind kind, t_sh_var *sh_var)
{
	int	io;

	if (kind == IO_INPUT || kind == IO_HEREDOC)
		io = 0;
	else
		io = 1;
	proc->kind[io] = kind;
	if (kind == IO_HEREDOC)
		return (0);
	if (proc->fd[io] != FD_NONE)
		safe_func(close(proc->fd[io]));
	if (set_io_filename(&proc->filename[io], \
						((t_token *)(itr->content))->str, sh_var))
		return (-1);
	proc->fd[io] = open_file(proc->filename[io], kind);
	if (proc->fd[io] == FD_NONE)
	{
		open_error_handler(proc->filename[io]);
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
			if (kind)
			{
				if (open_io_file(proc, itr, kind, sh_var) == -1)
					return (-1);
			}
		}
		itr = itr->next;
	}
	return (0);
}
