/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:59:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/11 12:29:49 by jkosaka          ###   ########.fr       */
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

static void	print_args(char **args, int i)
{
	while (args[i])
	{
		printf("%s", args[i]);
		i++;
		if (args[i])
			printf(" ");
	}
}

int	builtin_echo(t_proc *proc, t_sh_var *sh_var)
{
	bool			has_endl;
	int				i;
	char			**args;

	(void)sh_var;
	has_endl = true;
	args = proc->command;
	if (args[1] == NULL)
	{
		printf("\n");
		return (fd_error_handler("echo"));
	}
	if (is_valid_option(args[1]))
		has_endl = false;
	i = 1;
	while (args[i] && is_valid_option(args[i]))
		i++;
	print_args(args, i);
	if (has_endl)
		printf("\n");
	return (fd_error_handler("echo"));
}
