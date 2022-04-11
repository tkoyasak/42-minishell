/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_environ.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:52 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/11 11:25:04 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_environ(t_sh_var *sh_var)
{
	char	**environ;
	t_list	*itr;
	size_t	i;
	t_env	*env;
	char	*key;

	i = 0;
	environ = ft_xcalloc(ft_lstsize(sh_var->env_list), sizeof(char *));
	itr = sh_var->env_list->next;
	while (itr)
	{
		env = itr->content;
		if (!env->val)
		{
			itr = itr->next;
			continue ;
		}
		key = ft_xstrjoin(env->key, "=");
		environ[i] = ft_xstrjoin(key, env->val);
		free(key);
		itr = itr->next;
		i++;
	}
	return (environ);
}
