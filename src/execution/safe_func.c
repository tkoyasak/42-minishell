#include "minishell.h"

ssize_t	safe_func(ssize_t res)
{
	if (res == -1)
		exit(EXIT_FAILURE);
	return (res);
}
