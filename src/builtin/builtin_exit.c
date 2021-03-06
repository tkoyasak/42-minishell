/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkosaka <jkosaka@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/24 13:59:56 by jkosaka           #+#    #+#             */
/*   Updated: 2022/04/11 12:38:23 by jkosaka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_over(unsigned long n, int m, int sign)
{
	if (sign > 0)
		return (n > (unsigned long)(LONG_MAX - m) / 10);
	else
		return (n > ((unsigned long)LONG_MAX + 1 - m) / 10);
}

static void	skip_space(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}

/*  convert to unsigned char  */
static int	a_to_uchar(char *str)
{
	unsigned long	ret;
	int				sign;

	sign = 1;
	ret = 0;
	skip_space(&str);
	if (*str == '+' || *str == '-')
	{
		sign = 44 - *str;
		str++;
	}
	if (ft_isdigit(*str) == false)
		return (-1);
	while (ft_isdigit(*str))
	{
		if (is_over(ret, *str - '0', sign))
			return (-1);
		ret = ret * 10 + (*str - '0');
		str++;
	}
	skip_space(&str);
	if (*str)
		return (-1);
	return ((unsigned char)(ret * sign));
}

int	builtin_exit(t_proc *proc, t_sh_var *sh_var)
{
	char	**command;

	(void)sh_var;
	ft_putendl_fd("exit", STDERR_FILENO);
	command = proc->command;
	if (command[1] == NULL)
		exit(g_exit_status);
	g_exit_status = a_to_uchar(command[1]);
	if (g_exit_status == -1)
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(command[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		g_exit_status = EXIT_OUT_RANGE;
	}
	else if (command[2])
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
		return (1);
	}
	exit(g_exit_status);
}
