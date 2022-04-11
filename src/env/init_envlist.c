/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envlist.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:55 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/11 11:24:21 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*consume_new_env(t_list *itr, char *str)
{
	t_env	*env;

	env = ft_xcalloc(1, sizeof(t_env));
	if (!ft_strchr(str, '='))
	{
		env->key = ft_strdup(str);
		env->val = NULL;
	}
	else
	{
		env->key = ft_xstrndup(str, ft_strchr(str, '=') - str);
		env->val = ft_xstrdup(ft_strchr(str, '=') + 1);
	}
	if (ft_strcmp(env->key, "OLDPWD") == 0)
	{
		free(env->val);
		env->val = NULL;
	}
	itr->next = ft_xlstnew(env);
	return (itr->next);
}

t_list	*init_envlist(void)
{
	t_list		*env_list;
	t_list		*itr;
	extern char	**environ;
	char		**env;

	env_list = ft_xlstnew(NULL);
	if (!environ)
		return (env_list);
	env = environ;
	itr = env_list;
	while (*env)
	{
		itr = consume_new_env(itr, *env);
		env++;
	}
	return (env_list);
}
