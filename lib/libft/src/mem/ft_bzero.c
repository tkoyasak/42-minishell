/*
** Writes 'n' zeroed bytes to string 's'.
** Does nothing if 'n' is zero.
*/

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	ft_memset(s, 0, n);
}
