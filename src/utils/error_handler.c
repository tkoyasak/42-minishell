#include "minishell.h"

void	error_handler(char *str)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	perror(str);
	exit(EXIT_FAILURE);
}
