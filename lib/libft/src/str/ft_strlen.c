/*
** Computes length of string 's'.
** Returns the number of characters that precede terminating NUL character.
*/

#include "libft.h"

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (*s++)
		len++;
	return (len);
}
