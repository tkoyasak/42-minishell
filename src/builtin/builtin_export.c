/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkoyasak <tkoyasak@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/25 11:04:19 by tkoyasak          #+#    #+#             */
/*   Updated: 2022/03/25 11:04:20 by tkoyasak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	builtin_export_print(t_list *env_list)
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

static bool validate_args(char *arg)
{
	bool valid;

	valid = true;
	if (ft_strlen(arg) == 0)
		valid = false;
	if (valid)
	{
		while (ft_isalnum(*arg) || *arg == '_')
			arg++;
		if (*arg != '=')
			valid = false;
	}
	if (!valid)
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	}
	return (valid);
}

int	builtin_export(t_proc *proc, t_sh_var *sh_var)
{
	char	*arg;
	char	*key;
	char	*val;

	if (ft_lstsize((t_list *)(proc->token_list)) == 1)
		builtin_export_print(sh_var->env_list);
	else
	{
		arg = ((t_token *)(proc->token_list->next->content))->str;
		if (!validate_args(arg))
			return (1);
		key = ft_xstrndup(arg, ft_strchr(arg, '=') - arg);
		val = ft_xstrdup(ft_strchr(arg, '=') + 1);
		set_env_value(key, val, sh_var);
		free(key);
		free(val);
	}
	return (0);
}
