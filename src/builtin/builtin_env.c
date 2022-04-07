/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:13 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/07 14:43:41 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_proc *proc, t_sh_var *sh_var)
{
	t_list	*itr;

	(void)proc;
	itr = sh_var->env_list->next;
	while (itr)
	{
		if (((t_env *)(itr->content))->val)
		{
			printf("%s=%s\n", ((t_env *)(itr->content))->key, \
							((t_env *)(itr->content))->val);
		}
		itr = itr->next;
	}
	return (fd_error_handler("env"));
}
