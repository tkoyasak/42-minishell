/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:19 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/28 15:59:14 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	export_error(char *str)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(str, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static void	builtin_export_print(t_list *env_list)
{
	t_list	*itr;

	itr = env_list->next;
	while (itr)
	{
		printf("declare -x %s", ((t_env *)(itr->content))->key);
		if (((t_env *)(itr->content))->val)
			printf("=\"%s\"\n", ((t_env *)(itr->content))->val);
		else
			printf("\n");
		itr = itr->next;
	}
}

static int	validate_arg(char *arg)
{
	char	*itr;

	if (ft_strlen(arg) == 0)
		return (export_error(arg));
	itr = arg;
	if (ft_isdigit(*itr) || *itr == '=')
		return (export_error(arg));
	while (*itr && (ft_isalnum(*itr) || *itr == '_'))
		itr++;
	if (*itr == '\0' || *itr == '=')
		return (0);
	else
		return (export_error(arg));
}

static int	builtin_export_core(char *arg, t_sh_var *sh_var)
{
	int		ret;
	char	*key;
	char	*val;

	ret = 0;
	if (validate_arg(arg))
		ret = 1;
	else if (!ft_strchr(arg, '='))
		set_env_value(arg, NULL, sh_var);
	else
	{
		key = ft_xstrndup(arg, ft_strchr(arg, '=') - arg);
		val = ft_xstrdup(ft_strchr(arg, '=') + 1);
		set_env_value(key, val, sh_var);
		free(key);
		free(val);
	}
	return (ret);
}

int	builtin_export(t_proc *proc, t_sh_var *sh_var)
{
	t_list	*itr;
	int		ret;

	ret = 0;
	if (ft_lstsize((t_list *)(proc->token_list)) == 1)
	{
		sort_env_list(sh_var->env_list->next);
		builtin_export_print(sh_var->env_list);
	}
	else
	{
		itr = proc->token_list->next;
		while (itr)
		{
			if (builtin_export_core(((t_token *)(itr->content))->str, sh_var))
				ret = 1;
			itr = itr->next;
		}
	}
	return (ret);
}
