/*
** Returns zero if 'c' tests false,
** or returns non-zero if 'c' tests true.
*/

#include "libft.h"

int	ft_isalpha(int c)
{
	return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}
