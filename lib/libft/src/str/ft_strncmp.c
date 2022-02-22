/*
** Compares 's1' and 's2' in lexicographical order,
** but not more than 'n' characters.
** Characters that appear after `\0' character are not compared
** (since strncmp() is designed for comparing strings rather than binary data).
** Comparison is done using unsigned characters,
** so that `\200' is greater than `\0'.
*/

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (n-- && (*p1 || *p2))
		if (*p1++ != *p2++)
			return (*--p1 - *--p2);
	return (0);
}
