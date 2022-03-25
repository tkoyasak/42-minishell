/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:24 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:04:25 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		builtin_unset(t_proc *proc, t_sh_var *sh_var)
{
	t_list	*itr;
	t_list	*next;
	t_list	*prev;
	char	*key;

	if (proc->token_list->next == NULL)
		return (0);
	key = ((t_token *)(proc->token_list->next->content))->str;
	prev = sh_var->env_list;
	itr = sh_var->env_list->next;
	while (itr)
	{
		next = itr->next;
		if (ft_strcmp(((t_env *)(itr->content))->key, key) == 0)
		{
			prev->next = next;
			free(((t_env *)(itr->content))->key);
			free(((t_env *)(itr->content))->val);
			free(itr->content);
			free(itr);
			break ;
		}
		prev = itr;
		itr = next;
	}
	return (0);
}
