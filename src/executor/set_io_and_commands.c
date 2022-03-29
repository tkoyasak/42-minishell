/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_and_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:52:19 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/30 00:03:02 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_io_kind	get_io_kind(char *redirect_str)
{
	const char	*redirect_strs[] = {"", "<", "<<", ">", ">>"};
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

static void	remove_one_token(t_list **itr)
{
	t_list	*tmp;

	tmp = (*itr)->next;
	ft_lstdelone(*itr, delete_token);
	*itr = tmp;
}

static void	remove_io_token(t_proc *proc)
{
	t_list	*itr;
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

static void	set_command(t_proc *proc)
{
	t_list	*itr;
	int		cmd_idx;

	if (ft_lstsize(proc->token_list) == 0)
		return ;
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

/*  set input/output parameters ans commands  */
int	set_io_and_commands(t_proc *proc, t_sh_var *sh_var)
{
	if (set_io_params(proc, sh_var))
		return (1);
	remove_io_token(proc);
	set_command(proc);
	return (0);
}
