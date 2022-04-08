/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:19 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/04/08 20:26:14 by jkosaka          ###   ########.fr       */
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
	if (ft_isdigit(*itr) || *itr == '=' || *itr == '+')
		return (export_error(arg));
	while (*itr && (ft_isalnum(*itr) || *itr == '_'))
		itr++;
	if (*itr == '\0' || *itr == '=' || !ft_strncmp(itr, "+=", 2))
		return (0);
	else
		return (export_error(arg));
}

static int	builtin_export_core(char *arg, t_sh_var *sh_var)
{
	char	*itr;
	char	*key;
	char	*val;

	if (validate_arg(arg))
		return (1);
	if (!ft_strchr(arg, '='))
		set_env_value(arg, NULL, sh_var);
	else
	{
		itr = arg;
		while (*itr && (ft_isalnum(*itr) || *itr == '_'))
			itr++;
		key = ft_xstrndup(arg, itr - arg);
		val = ft_xstrdup(itr + 1 + (*itr == '+'));
		if (*itr == '=')
			set_env_value(key, val, sh_var);
		else
			append_env_value(key, val, sh_var);
		free(key);
		free(val);
	}
	return (0);
}

int	builtin_export(t_proc *proc, t_sh_var *sh_var)
{
	int		ret;
	int		idx;

	idx = 1;
	if (proc->command[idx] == NULL)
	{
		sort_env_list(sh_var->env_list->next);
		builtin_export_print(sh_var->env_list);
		return (fd_error_handler("export"));
	}
	else
	{
		ret = 0;
		while (proc->command[idx])
		{
			if (builtin_export_core(proc->command[idx], sh_var))
				ret = 1;
			idx++;
		}
		return (ret);
	}
}
