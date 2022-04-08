/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:24 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/08 20:28:19 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	unset_error(char *str)
{
	ft_putstr_fd("minishell: unset: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static int	validate_arg(char *arg)
{
	char	*itr;

	if (ft_strlen(arg) == 0)
		return (unset_error(arg));
	itr = arg;
	if (ft_isdigit(*itr))
		return (unset_error(arg));
	while (*itr && (ft_isalnum(*itr) || *itr == '_'))
		itr++;
	if (*itr == '\0')
		return (0);
	else
		return (unset_error(arg));
}

static int	builtin_unset_core(char *key, t_sh_var *sh_var)
{
	t_list	*itr;
	t_list	*next;
	t_list	*prev;

	if (validate_arg(key))
		return (1);
	prev = sh_var->env_list;
	itr = sh_var->env_list->next;
	while (itr)
	{
		next = itr->next;
		if (ft_strcmp(((t_env *)(itr->content))->key, key) == 0)
		{
			prev->next = next;
			ft_lstdelone(itr, delete_env);
			break ;
		}
		prev = itr;
		itr = next;
	}
	return (0);
}

int	builtin_unset(t_proc *proc, t_sh_var *sh_var)
{
	int		ret;
	int		idx;

	idx = 1;
	ret = 0;
	while (proc->command[idx])
	{
		if (builtin_unset_core(proc->command[idx], sh_var))
			ret = 1;
		idx++;
	}
	return (ret);
}
