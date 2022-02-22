/*
** Returns zero if 'c' tests false,
** or returns non-zero if 'c' tests true.
*/

#include "libft.h"

int	ft_isascii(int c)
{
	return (0 <= c && c <= 127);
}
