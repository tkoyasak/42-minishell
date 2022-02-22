#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	unsigned char	*p1;
	unsigned char	*p2;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (*p1 && *p2)
		if (*p1++ != *p2++)
			return (*--p1 - *--p2);
	return (*p1 - *p2);
}
