/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:57 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:04:58 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_env_value(char *key, char *val, t_sh_var *sh_var)
{
	t_list	*itr;
	t_env	*env;

	if (ft_strcmp(key, "?") == 0)
		return ;
	itr = sh_var->env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)itr->content)->key, key) == 0)
		{
			free(((t_env *)(itr->content))->val);
			((t_env *)(itr->content))->val = ft_xstrdup(val);
			return ;
		}
		itr = itr->next;
	}
	if (itr == NULL)
	{
		env = ft_xcalloc(1, sizeof(t_env));
		env->key = ft_xstrdup(key);
		env->val = ft_xstrdup(val);
		ft_lstadd_back(&(sh_var->env_list), ft_xlstnew(env));
	}
}