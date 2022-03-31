/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:59:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/03/31 11:05:34 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_valid_option(char *str)
{
	if (!str)
		return (false);
	if (*str != '-')
		return (false);
	str++;
	if (*str != 'n')
		return (false);
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

int	builtin_echo(t_proc *proc, t_sh_var *sh_var)
{
	t_list			*itr;
	bool			has_endl;

	(void)sh_var;
	has_endl = true;
	itr = proc->token_list->next;
	if (itr == NULL)
	{
		printf("\n");
		return (0);
	}
	if (is_valid_option(((t_token *)(itr->content))->str))
		has_endl = false;
	while (itr && is_valid_option(((t_token *)(itr->content))->str))
		itr = itr->next;
	while (itr)
	{
		printf("%s", ((t_token *)(itr->content))->str);
		itr = itr->next;
		if (itr)
			printf(" ");
	}
	if (has_endl)
		printf("\n");
	return (0);
}
