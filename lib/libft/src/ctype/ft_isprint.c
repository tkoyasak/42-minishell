/*
** Returns zero if 'c' tests false,
** or returns non-zero if 'c' tests true.
*/

#include "libft.h"

int	ft_isprint(int c)
{
	return (32 <= c && c <= 126);
}
