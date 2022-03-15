#include "minishell.h"

static int	is_over(unsigned long n, int m, int sign)
{
	if (sign > 0)
		return (n > (LONG_MAX - m) / 10);
	else
		return (n > ((unsigned long)LONG_MAX + 1 - m) / 10);
}

int	a_to_uchar(const char *str)
{
	unsigned long	ret;
	int		sign;

	sign = 1;
	ret = 0;
	while (ft_isspace(*str))
		str++;
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
	return ((unsigned char)(ret * sign));
}

int		builtin_exit(t_expression *expression, t_process *process, t_shell_var *shell_var)
{
	char	**command;
	// int		exit_status;

	command = process->command;
	if (command[1] == NULL)
		exit(0);
	g_exit_status = a_to_uchar(command[1]);
	// printf("exit_status: %d\n", exit_status);
	if (g_exit_status == -1)
	{
		ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
		g_exit_status = 255;
	}
	else if (command[2])
	{
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
		g_exit_status = 1;
	}
	exit(g_exit_status);
}
