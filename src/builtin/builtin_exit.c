#include "builtin.h"

static int	is_over(long n, int m, int sign)
{
	if (sign > 0)
		return (n > (LONG_MAX - m) / 10);
	else
		return (n > (-LONG_MIN - m) / 10);
}

int	a_to_uchar(const char *str)
{
	long	ret;
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
	int		exit_status;

	command = process->command;
	if (command[1] == NULL)
		return (0);
	exit_status = a_to_uchar(command[1]);
	// printf("exit_status: %d\n", exit_status);
	if (exit_status == -1)
	{
		ft_putstr_fd("numeric argument required\n", STDERR_FILENO);
		return (255);
	}
	if (command[2])
	{
		ft_putstr_fd("too many arguments\n", STDERR_FILENO);
		return (1);
	}
	return (exit_status);
}
