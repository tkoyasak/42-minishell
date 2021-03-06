/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_env_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:50 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:52:52 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *key, t_sh_var *sh_var)
{
	t_list	*itr;

	if (ft_strcmp(key, "?") == 0)
		return (ft_xitoa(g_exit_status));
	itr = sh_var->env_list->next;
	while (itr)
	{
		if (ft_strcmp(((t_env *)itr->content)->key, key) == 0)
		{
			if (((t_env *)(itr->content))->val)
				return (ft_xstrdup(((t_env *)(itr->content))->val));
			else
				return (NULL);
		}
		itr = itr->next;
	}
	return (NULL);
}

char	*get_env_value_str(char *key, t_sh_var *sh_var)
{
	char	*val;

	val = get_env_value(key, sh_var);
	if (val)
		return (val);
	else
		return (ft_xstrdup(""));
}
