/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:57 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/28 14:59:04 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*set_new_env(char *key, char *val)
{
	t_env	*env;

	env = ft_xcalloc(1, sizeof(t_env));
	env->key = ft_xstrdup(key);
	if (val)
		env->val = ft_xstrdup(val);
	else
		env->val = NULL;
	return (ft_xlstnew(env));
}

void	set_env_value(char *key, char *val, t_sh_var *sh_var)
{
	t_list	*itr;
	t_env	*env;
	char	*old_val;

	if (ft_strcmp(key, "?") == 0)
		return ;
	itr = sh_var->env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)itr->content)->key, key) == 0)
		{
			old_val = ((t_env *)(itr->content))->val;
			if (val)
			{
				((t_env *)(itr->content))->val = ft_xstrdup(val);
				free(old_val);
			}
			return ;
		}
		itr = itr->next;
	}
	if (itr == NULL)
		ft_lstadd_back(&(sh_var->env_list), set_new_env(key, val));
}
