/*
** Writes 'len' bytes of value 'c'(converted to unsigned char) to string 'b'.
** Returns its first argument.
*/

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;

	p = b;
	while (len--)
		*p++ = (unsigned char)c;
	return ((void *)b);
}
