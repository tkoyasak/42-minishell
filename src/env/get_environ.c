/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_environ.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:52 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:04:53 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_environ(t_sh_var *sh_var)
{
	char	**environ;
	t_list	*itr;
	size_t	i;
	char	*key;

	i = 0;
	environ = ft_xcalloc(ft_lstsize(sh_var->env_list), sizeof(char *));
	itr = sh_var->env_list->next;
	while (itr)
	{
		key = ft_strjoin(((t_env *)itr->content)->key, "=");
		environ[i] = ft_strjoin(key, ((t_env *)itr->content)->val);
		free(key);
		itr = itr->next;
		i++;
	}
	return (environ);
}
