/*
** Returns zero if 'c' tests false,
** or returns non-zero if 'c' tests true.
*/

#include "libft.h"

int	ft_isdigit(int c)
{
	return ('0' <= c && c <= '9');
}
