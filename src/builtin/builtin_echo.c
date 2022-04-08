/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:59:17 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/08 20:29:39 by jkosaka          ###   ########.fr       */
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
	bool			has_endl;
	int				i;
	char			**command;

	(void)sh_var;
	has_endl = true;
	command = proc->command;
	if (command[1] == NULL)
	{
		printf("\n");
		return (fd_error_handler("echo"));
	}
	if (is_valid_option(command[1]))
		has_endl = false;
	i = 1;
	while (command[i] && is_valid_option(command[i]))
		i++;
	while (command[i])
	{
		printf("%s", command[i]);
		i++;
		if (command[i])
			printf(" ");
	}
	if (has_endl)
		printf("\n");
	return (fd_error_handler("echo"));
}
