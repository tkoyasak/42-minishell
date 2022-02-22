/*
** Compares 's1' against 's2', both strings are assumed 'n' bytes long.
** Returns zero if two strings are identical or if 'n' is zero
** (because zero-length strings are also always identical).
** Otherwise, returns difference between the first two differing bytes
** (treated as unsigned char values, i.e `\200' is greater than `\0').
*/

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (n--)
		if (*p1++ != *p2++)
			return (*--p1 - *--p2);
	return (0);
}
