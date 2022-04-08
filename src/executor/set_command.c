/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_io_and_commands.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 11:52:19 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 16:45:53 by jkosaka          ###   ########.fr       */
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

void	set_command(t_proc *proc)
{
	t_list	*itr;
	int		list_size;
	int		cmd_idx;
	t_token	*token;

	list_size = ft_lstsize(proc->token_list);
	proc->command = ft_xcalloc(list_size + 1, sizeof(char *));
	itr = proc->token_list;
	cmd_idx = 0;
	while (itr)
	{
		token = itr->content;
		if (token->kind == TK_IO)
			itr = itr->next;
		else
		{
			proc->command[cmd_idx] = token->str;
			cmd_idx++;
		}
		itr = itr->next;
	}
}
