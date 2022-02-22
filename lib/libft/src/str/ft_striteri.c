/*
** Applies function 'f' to each character of string passed as argument,
** and passing its index as first argument.
** Each character is passed by address to 'f' to be modified if necessary.
*/

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	size_t	slen;
	size_t	i;

	if (!s || !f)
		return ;
	slen = ft_strlen(s);
	i = 0;
	while (i < slen)
	{
		f(i, (char *)s + i);
		i++;
	}
}
